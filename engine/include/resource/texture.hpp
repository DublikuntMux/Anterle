#pragma once

#include <cstdint>

namespace Anterle {
class Texture2D
{
public:
  Texture2D();

  void Generate(uint32_t width, uint32_t height, void *data);
  void Bind() const;

public:
  uint32_t ID{};
  uint32_t Width, Height;
  int Internal_Format;
  uint32_t Image_Format;
  int Wrap_S;
  int Wrap_T;
  int Filter_Min;
  int Filter_Max;
};
}// namespace Anterle
