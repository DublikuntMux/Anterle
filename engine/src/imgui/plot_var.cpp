#include <algorithm>
#include <cassert>
#include <map>

#include <imgui.h>

#include "imgui/plot_var.hpp"

struct PlotVarData
{
  ImGuiID ID;
  ImVector<float> Data;
  int DataInsertIdx;
  int LastFrame;

  PlotVarData() : ID(0), DataInsertIdx(0), LastFrame(0) {}
};

typedef std::map<ImGuiID, PlotVarData> PlotVarsMap;
static PlotVarsMap g_PlotVarsMap;

void ImGui::PlotVar(const char *label, float value, float scale_min, float scale_max, int buffer_size)
{
  assert(label);
  if (buffer_size == 0) buffer_size = 120;

  ImGui::PushID(label);
  ImGuiID id = ImGui::GetID("");

  PlotVarData &pvd = g_PlotVarsMap[id];

  if (pvd.Data.capacity() != buffer_size) {
    pvd.Data.resize(buffer_size);
    memset(&pvd.Data[0], 0, sizeof(float) * buffer_size);
    pvd.DataInsertIdx = 0;
    pvd.LastFrame = 0;
  }

  if (pvd.DataInsertIdx == buffer_size) pvd.DataInsertIdx = 0;
  int display_idx = pvd.DataInsertIdx;
  if (value != FLT_MAX) pvd.Data[pvd.DataInsertIdx++] = value;

  int current_frame = ImGui::GetFrameCount();
  if (pvd.LastFrame != current_frame) {
    ImGui::PlotLines("##plot", &pvd.Data[0], buffer_size, pvd.DataInsertIdx, NULL, scale_min, scale_max, ImVec2(0, 40));
    ImGui::SameLine();
    ImGui::Text("%s\n%-3.4f", label, pvd.Data[display_idx]);
    pvd.LastFrame = current_frame;
  }

  ImGui::PopID();
}

void ImGui::PlotVarFlushOldEntries()
{
  int current_frame = ImGui::GetFrameCount();
  for (PlotVarsMap::iterator it = g_PlotVarsMap.begin(); it != g_PlotVarsMap.end();) {
    PlotVarData &pvd = it->second;
    if (pvd.LastFrame < current_frame - std::max(400, (int)pvd.Data.size()))
      it = g_PlotVarsMap.erase(it);
    else
      ++it;
  }
}