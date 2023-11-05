#pragma once

#include <stdint.h>

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
  uint32_t m_width;
  uint32_t m_height;
  uint32_t m_top;
  uint32_t m_left;
  ClickCallback m_onClick;

  Shader m_shader;
  uint32_t m_quadVAO;
};
}// namespace Anterle
