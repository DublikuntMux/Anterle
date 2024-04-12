#pragma once

#include <anterle_engine.hpp>

#include "AnterleGame.hpp"

class AnterleWindow : public Anterle::Window
{
public:
  AnterleWindow(int screen_width, int screen_Height, const char *name, AnterleGame *instance);
};
