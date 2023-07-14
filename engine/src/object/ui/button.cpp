#include "object/ui/button.hpp"

Button::Button(uint32_t top, uint32_t left, uint32_t width, uint32_t height, ClickCallback onClick)
  : m_top(top), m_left(left), m_width(width), m_height(height), m_onClick(onClick)
{}

Button::~Button() {}

void Button::CheckClick(uint32_t x, uint32_t y)
{
  if (x >= this->m_left && x <= this->m_left + this->m_width && y >= this->m_top && y <= this->m_top + this->m_height) {
    this->m_onClick();
  }
}
