#pragma once

#include <array>
#include <chrono>

#include <imgui.h>

class Profiler
{
public:
  enum Stage {
    PollEvents,
    GameEvents,
    PressInput,
    Update,
    ImguiNewFrame,
    OpenGL,
    GameRender,
    ImguiRender,
    SwapBuffer,

    _StageCount,
  };

  struct Scope
  {
    ImU8 level;
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;
    bool finalized = false;

    Scope() = default;

    Scope(const ImU8 &level,
      const std::chrono::system_clock::time_point &start,
      const std::chrono::system_clock::time_point &end,
      bool finalized)
      : level(level), start(start), end(end), finalized(finalized)
    {}

    bool operator==(const Scope &other) const
    {
      return level == other.level && start == other.start && end == other.end && finalized == other.finalized;
    }
  };

  struct Entry
  {
    std::chrono::system_clock::time_point frameStart;
    std::chrono::system_clock::time_point frameEnd;
    std::array<Scope, _StageCount> stages;

    Entry() = default;

    Entry(const std::chrono::system_clock::time_point &frameStart,
      const std::chrono::system_clock::time_point &frameEnd,
      const std::array<Scope, _StageCount> &stages)
      : frameStart(frameStart), frameEnd(frameEnd), stages(stages)
    {}

    bool operator==(const Entry &other) const
    {
      return frameStart == other.frameStart && frameEnd == other.frameEnd && stages == other.stages;
    }
  };

  static const ImU8 bufferSize = 100;
  std::array<Entry, bufferSize> entries;

  Profiler() = default;

  Profiler(const std::array<Entry, bufferSize> &entries, const ImU8 &_currentEntry, const ImU8 &_currentLevel)
    : entries(entries), _currentEntry(_currentEntry), _currentLevel(_currentLevel)
  {}

  void Frame()
  {
    auto &prevEntry = entries[_currentEntry];
    _currentEntry = (_currentEntry + 1) % bufferSize;
    prevEntry.frameEnd = entries[_currentEntry].frameStart = std::chrono::system_clock::now();
  }

  void Begin(Stage stage)
  {
    assert(_currentLevel < 255);
    auto &entry = entries[_currentEntry].stages[stage];
    entry.level = _currentLevel;
    _currentLevel++;
    entry.start = std::chrono::system_clock::now();
    entry.finalized = false;
  }

  void End(Stage stage)
  {
    assert(_currentLevel > 0);
    auto &entry = entries[_currentEntry].stages[stage];
    assert(!entry.finalized);
    _currentLevel--;
    assert(entry.level == _currentLevel);
    entry.end = std::chrono::system_clock::now();
    entry.finalized = true;
  }

  ImU8 GetCurrentEntryIndex() const { return (_currentEntry + bufferSize - 1) % bufferSize; }

  bool operator==(const Profiler &other) const
  {
    return entries == other.entries && _currentEntry == other._currentEntry && _currentLevel == other._currentLevel;
  }

private:
  ImU8 _currentEntry = bufferSize - 1;
  ImU8 _currentLevel = 0;
};

static const std::array<const char *, Profiler::_StageCount> stageNames = {
  "Poll Events",
  "Game Events",
  "Press Input",
  "Update",
  "Imgui new frame",
  "OpenGL",
  "Game Render",
  "Imgui Render",
  "Swap Buffer",
};

void ProfilerValueGetter(float *startTimestamp,
  float *endTimestamp,
  ImU8 *level,
  const char **caption,
  const void *data,
  int idx);
void PlotFlame(const char *label,
  void (*values_getter)(float *start, float *end, ImU8 *level, const char **caption, const void *data, int idx),
  const void *data,
  int values_count,
  int values_offset = 0,
  const char *overlay_text = NULL,
  float scale_min = FLT_MAX,
  float scale_max = FLT_MAX,
  ImVec2 graph_size = ImVec2(0, 0));
