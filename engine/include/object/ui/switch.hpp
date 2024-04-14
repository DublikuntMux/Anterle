#pragma once

#include <cstdint>

#include "object/ui/clickable.hpp"

namespace Anterle {
class Switch : Clickable
{
public:
  Switch(uint32_t top, uint32_t left, uint32_t width, uint32_t height, ClickCallback onClick);

  void Render() override;
};
}// namespace Anterle
