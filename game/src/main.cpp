#include <iostream>

#include <anterle_engine.hpp>

#include "AnterleGame.hpp"
#include "AnterleWindow.hpp"

const uint32_t SCREEN_WIDTH = 1280;
const uint32_t SCREEN_HEIGHT = 720;

int main()
{
  AnterleGame game(SCREEN_WIDTH, SCREEN_HEIGHT);
  AnterleWindow window(SCREEN_WIDTH, SCREEN_HEIGHT, "Anterle", &game);

  window.Init();
  window.Start();

  std::cin.get();
}
