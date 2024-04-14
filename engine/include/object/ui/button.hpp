#pragma once

#include <cstdint>

#include "object/ui/clickable.hpp"

namespace Anterle {
class Button : Clickable
{
public:
  Button(uint32_t top, uint32_t left, uint32_t width, uint32_t height, ClickCallback onClick);
  void Render();
};
}// namespace Anterle
