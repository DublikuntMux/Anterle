#pragma once

#include <array>
#include <chrono>

#include <imgui.h>

namespace Anterle {
class Profiler
{
public:
  enum Stage {
    PollEvents,
    GameEvents,
    PressInput,
    FixedUpdate,
    Update,
    ImguiNewFrame,
    CutomTitleBar,
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
  };

  struct Entry
  {
    std::chrono::system_clock::time_point frameStart;
    std::chrono::system_clock::time_point frameEnd;
    std::array<Scope, _StageCount> stages;
  };

  static const ImU8 bufferSize = 100;
  std::array<Entry, bufferSize> entries;

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

private:
  ImU8 _currentEntry = bufferSize - 1;
  ImU8 _currentLevel = 0;
};

static const std::array<const char *, Profiler::_StageCount> stageNames = {
  "Poll Events",
  "Game Events",
  "Press Input",
  "Fixed Update",
  "Update",
  "Imgui new frame",
  "Cutom Title Bar",
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
  size_t idx);
void PlotFlame(const char *label,
  void (*values_getter)(float *start, float *end, ImU8 *level, const char **caption, const void *data, size_t idx),
  const void *data,
  int values_count,
  int values_offset = 0,
  const char *overlay_text = NULL,
  float scale_min = FLT_MAX,
  float scale_max = FLT_MAX,
  ImVec2 graph_size = ImVec2(0, 0));

}// namespace Anterle
