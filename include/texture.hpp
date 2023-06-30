#pragma once

#include <cstdint>

class Texture2D {
public:
  uint32_t ID;
  uint32_t Width, Height;
  uint32_t Internal_Format;
  uint32_t Image_Format;
  uint32_t Wrap_S;
  uint32_t Wrap_T;
  uint32_t Filter_Min;
  uint32_t Filter_Max;

  Texture2D();

  void Generate(uint32_t width, uint32_t height, unsigned char *data);
  void Bind() const;
};
