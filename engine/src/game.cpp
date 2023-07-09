#include <stdint.h>

#include "game.hpp"

Game::Game(uint16_t width, uint16_t height) : State(GAME_MENU), Width(width), Height(height), Keys(), KeysProcessed() {}
