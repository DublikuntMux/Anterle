#include <anterle_engine.hpp>

#include "AnterleGame.hpp"
#include "AnterleWindow.hpp"

constexpr uint32_t SCREEN_WIDTH = 1280;
constexpr uint32_t SCREEN_HEIGHT = 720;

int main()
{
  AnterleGame game(SCREEN_WIDTH, SCREEN_HEIGHT);
  AnterleWindow window(SCREEN_WIDTH, SCREEN_HEIGHT, "Anterle", &game);

  window.Init();
  window.Start();

  return 0;
}
