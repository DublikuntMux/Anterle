#include "object/ui/switch.hpp"
#include "object/ui/clickable.hpp"

namespace Anterle {
Switch::Switch(uint32_t top, uint32_t left, uint32_t width, uint32_t height, ClickCallback onClick)
  : Clickable(top, left, width, height, onClick)
{}

void Switch::Render() {}

}// namespace Anterle
