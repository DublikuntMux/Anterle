#pragma once

enum class MsaaLevel { None, X2, X4, X8, X16 };

typedef struct
{
  MsaaLevel msaaLevel;
} RenderSettings;
