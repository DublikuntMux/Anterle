#pragma once

#include <stdint.h>

class Button
{
public:
  typedef void (*ClickCallback)();

  Button(uint32_t top, uint32_t left, uint32_t width, uint32_t height, ClickCallback onClick);
  ~Button();

  void CheckClick(uint32_t x, uint32_t y);

private:
  uint32_t m_width;
  uint32_t m_height;
  uint32_t m_top;
  uint32_t m_left;
  ClickCallback m_onClick;
};
