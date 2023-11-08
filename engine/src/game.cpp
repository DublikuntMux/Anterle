#include <stdint.h>

#include "game.hpp"

namespace Anterle {
Game::Game(uint16_t width, uint16_t height)
  : State(GameState::GAME_MENU), Width(width), Height(height), Keys(), KeysProcessed()
{}
Game::~Game() {}

void Game::Init() {}
void Game::Update() {}
void Game::FixedUpdate() {}
void Game::ProcessInput() {}

void Game::Render() {}

void Game::ResetLevel() {}
}// namespace Anterle
