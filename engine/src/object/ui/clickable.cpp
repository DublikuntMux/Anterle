#include "object/ui/clickable.hpp"

namespace Anterle {
Clickable::Clickable(uint32_t top, uint32_t left, uint32_t width, uint32_t height, ClickCallback onClick)
  : m_width(width), m_height(height), m_top(top), m_left(left), m_onClick(onClick)
{}

void Clickable::CheckClick(uint32_t x, uint32_t y)
{
  if (x >= this->m_left && x <= this->m_left + this->m_width && y >= this->m_top && y <= this->m_top + this->m_height) {
    this->m_onClick();
  }
}
}// namespace Anterle
