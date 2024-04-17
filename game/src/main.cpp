#include <cpptrace/cpptrace.hpp>
#include <anterle_engine.hpp>

#include "AnterleGame.hpp"

constexpr uint32_t SCREEN_WIDTH = 1280;
constexpr uint32_t SCREEN_HEIGHT = 720;

int main()
{
  cpptrace::register_terminate_handler();

  AnterleGame game(SCREEN_WIDTH, SCREEN_HEIGHT, "Anterle");
  game.Init();
  game.Start();

  return 0;
}
