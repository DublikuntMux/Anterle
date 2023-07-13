#pragma once

#include <anterle_engine.hpp>

#include "AnterleGame.hpp"

class AnterleWindow : public Window
{
public:
  AnterleWindow(uint32_t screen_width, uint32_t screen_hight, const char *name, AnterleGame *instance);
};
