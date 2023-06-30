#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include "profiler.hpp"

void ProfilerValueGetter(float *startTimestamp, float *endTimestamp,
                         ImU8 *level, const char **caption, const void *data,
                         int idx) {
  auto entry = reinterpret_cast<const Profiler::Entry *>(data);
  auto &stage = entry->stages[idx];
  if (startTimestamp) {
    std::chrono::duration<float, std::milli> fltStart =
        stage.start - entry->frameStart;
    *startTimestamp = fltStart.count();
  }
  if (endTimestamp) {
    *endTimestamp = stage.end.time_since_epoch().count() / 1000000.0f;

    std::chrono::duration<float, std::milli> fltEnd =
        stage.end - entry->frameStart;
    *endTimestamp = fltEnd.count();
  }
  if (level) {
    *level = stage.level;
  }
  if (caption) {
    *caption = stageNames[idx];
  }
}

void PlotFlame(const char *label,
               void (*values_getter)(float *start, float *end, ImU8 *level,
                                     const char **caption, const void *data,
                                     int idx),
               const void *data, int values_count, int values_offset,
               const char *overlay_text, float scale_min, float scale_max,
               ImVec2 graph_size) {
  ImGuiWindow *window = ImGui::GetCurrentWindow();
  if (window->SkipItems)
    return;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;

  ImU8 maxDepth = 0;
  for (int i = values_offset; i < values_count; ++i) {
    ImU8 depth;
    values_getter(nullptr, nullptr, &depth, nullptr, data, i);
    maxDepth = ImMax(maxDepth, depth);
  }

  const auto blockHeight =
      ImGui::GetTextLineHeight() + (style.FramePadding.y * 2);
  const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
  if (graph_size.x == 0.0f)
    graph_size.x = ImGui::CalcItemWidth();
  if (graph_size.y == 0.0f)
    graph_size.y = label_size.y + (style.FramePadding.y * 3) +
                   blockHeight * (maxDepth + 1);

  const ImRect frame_bb(window->DC.CursorPos,
                        window->DC.CursorPos + graph_size);
  const ImRect inner_bb(frame_bb.Min + style.FramePadding,
                        frame_bb.Max - style.FramePadding);
  const ImRect total_bb(frame_bb.Min,
                        frame_bb.Max +
                            ImVec2(label_size.x > 0.0f
                                       ? style.ItemInnerSpacing.x + label_size.x
                                       : 0.0f,
                                   0));
  ImGui::ItemSize(total_bb, style.FramePadding.y);
  if (!ImGui::ItemAdd(total_bb, 0, &frame_bb))
    return;

  if (scale_min == FLT_MAX || scale_max == FLT_MAX) {
    float v_min = FLT_MAX;
    float v_max = -FLT_MAX;
    for (int i = values_offset; i < values_count; i++) {
      float v_start, v_end;
      values_getter(&v_start, &v_end, nullptr, nullptr, data, i);
      if (v_start == v_start)
        v_min = ImMin(v_min, v_start);
      if (v_end == v_end)
        v_max = ImMax(v_max, v_end);
    }
    if (scale_min == FLT_MAX)
      scale_min = v_min;
    if (scale_max == FLT_MAX)
      scale_max = v_max;
  }

  ImGui::RenderFrame(frame_bb.Min, frame_bb.Max,
                     ImGui::GetColorU32(ImGuiCol_FrameBg), true,
                     style.FrameRounding);

  bool any_hovered = false;
  if (values_count - values_offset >= 1) {
    const ImU32 col_base =
        ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x77FFFFFF;
    const ImU32 col_hovered =
        ImGui::GetColorU32(ImGuiCol_PlotHistogramHovered) & 0x77FFFFFF;
    const ImU32 col_outline_base =
        ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x7FFFFFFF;
    const ImU32 col_outline_hovered =
        ImGui::GetColorU32(ImGuiCol_PlotHistogramHovered) & 0x7FFFFFFF;

    for (int i = values_offset; i < values_count; ++i) {
      float stageStart, stageEnd;
      ImU8 depth;
      const char *caption;

      values_getter(&stageStart, &stageEnd, &depth, &caption, data, i);

      auto duration = scale_max - scale_min;
      if (duration == 0) {
        return;
      }

      auto start = stageStart - scale_min;
      auto end = stageEnd - scale_min;

      auto startX = static_cast<float>(start / (double)duration);
      auto endX = static_cast<float>(end / (double)duration);

      float width = inner_bb.Max.x - inner_bb.Min.x;
      float height =
          blockHeight * (maxDepth - depth + 1) - style.FramePadding.y;

      auto pos0 = inner_bb.Min + ImVec2(startX * width, height);
      auto pos1 = inner_bb.Min + ImVec2(endX * width, height + blockHeight);

      bool v_hovered = false;
      if (ImGui::IsMouseHoveringRect(pos0, pos1)) {
        ImGui::SetTooltip("%s: %8.4g", caption, stageEnd - stageStart);
        v_hovered = true;
        any_hovered = v_hovered;
      }

      window->DrawList->AddRectFilled(pos0, pos1,
                                      v_hovered ? col_hovered : col_base);
      window->DrawList->AddRect(
          pos0, pos1, v_hovered ? col_outline_hovered : col_outline_base);
      auto textSize = ImGui::CalcTextSize(caption);
      auto boxSize = (pos1 - pos0);
      auto textOffset = ImVec2(0.0f, 0.0f);
      if (textSize.x < boxSize.x) {
        textOffset = ImVec2(0.5f, 0.5f) * (boxSize - textSize);
        ImGui::RenderText(pos0 + textOffset, caption);
      }
    }

    if (overlay_text)
      ImGui::RenderTextClipped(
          ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y),
          frame_bb.Max, overlay_text, NULL, NULL, ImVec2(0.5f, 0.0f));

    if (label_size.x > 0.0f)
      ImGui::RenderText(
          ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y),
          label);
  }

  if (!any_hovered && ImGui::IsItemHovered()) {
    ImGui::SetTooltip("Total: %8.4g", scale_max - scale_min);
  }
}
