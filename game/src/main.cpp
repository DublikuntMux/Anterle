#include <mimalloc.h>
#include <mimalloc-new-delete.h>
#include <mimalloc-override.h>

#include <cpptrace/cpptrace.hpp>

#include <SDL.h>
#include <anterle_engine.hpp>

#include "AnterleGame.hpp"

constexpr uint32_t SCREEN_WIDTH = 1280;
constexpr uint32_t SCREEN_HEIGHT = 720;

#undef main
int main()
{
  cpptrace::register_terminate_handler();

  SDL_SetMemoryFunctions(mi_malloc, mi_calloc, mi_realloc, mi_free);

  AnterleGame game(SCREEN_WIDTH, SCREEN_HEIGHT, "Anterle");
  game.Init();
  game.Start();

  return 0;
}
