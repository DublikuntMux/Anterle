#include <anterle_engine.hpp>

#include "AnterleGame.hpp"
#include "AnterleWindow.hpp"

AnterleWindow::AnterleWindow(int screen_width, int screen_hight, const char *name, AnterleGame *instance)
  : Window(screen_width, screen_hight, name, instance)
{}
