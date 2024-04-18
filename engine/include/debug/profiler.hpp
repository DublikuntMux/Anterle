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
    FixedUpdate,
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
    ImU8 level{};
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

  void Frame();
  void Begin(Stage stage);
  void End(Stage stage);

  Entry &getEntry(size_t index);
  void setEntry(size_t index, const Entry &entry);

  [[nodiscard]] ImU8 GetCurrentEntryIndex() const { return (_currentEntry + entries.size() - 1) % entries.size(); }

private:
  std::array<Entry, 100> entries;
  ImU8 _currentEntry = entries.size() - 1;
  ImU8 _currentLevel = 0;
};

static const std::array<const char *, Profiler::_StageCount> stageNames = {
  "Poll Events",
  "Game Events",
  "Fixed Update",
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
  size_t idx);

void PlotFlame(const char *label,
  void (*values_getter)(float *start, float *end, ImU8 *level, const char **caption, const void *data, size_t idx),
  const void *data,
  int values_count,
  int values_offset = 0,
  const char *overlay_text = nullptr,
  float scale_min = FLT_MAX,
  float scale_max = FLT_MAX,
  ImVec2 graph_size = ImVec2(0, 0));

}// namespace Anterle
