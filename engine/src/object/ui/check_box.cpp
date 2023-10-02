#include "object/ui/check_box.hpp"
#include "object/ui/clickable.hpp"

namespace Anterle {
CheckBox::CheckBox(uint32_t top, uint32_t left, uint32_t width, uint32_t height, ClickCallback onClick)
  : Clickable(top, left, width, height, onClick)
{}

void CheckBox::Render() {}

}// namespace Anterle
