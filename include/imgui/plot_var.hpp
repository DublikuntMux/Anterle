#pragma once

#include <cfloat>

namespace ImGui
{
void PlotVar(const char* label, float value, float scale_min = FLT_MAX, float scale_max = FLT_MAX, int buffer_size = 120);
void PlotVarFlushOldEntries();
}