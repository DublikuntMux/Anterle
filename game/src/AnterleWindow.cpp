#include "anterle_engine.hpp"

#include "AnterleGame.hpp"
#include "AnterleWindow.hpp"

AnterleWindow::AnterleWindow(uint32_t screen_width, uint32_t screen_hight, const char *name, AnterleGame *instance)
  : Window(screen_width, screen_hight, name, instance)
{}
