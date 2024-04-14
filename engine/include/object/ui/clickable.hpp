#pragma once

#include <cstdint>

#include "resource/shader.hpp"

namespace Anterle {
class Clickable
{
public:
  typedef void (*ClickCallback)();

  Clickable(uint32_t top, uint32_t left, uint32_t width, uint32_t height, ClickCallback onClick);
  virtual ~Clickable();

  virtual void CheckClick(double x, double y);
  virtual void Render();

private:
  uint32_t _width;
  uint32_t _height;
  uint32_t _top;
  uint32_t _left;
  ClickCallback _onClick;

  Shader _shader;
  uint32_t _quadVAO;
};
}// namespace Anterle
