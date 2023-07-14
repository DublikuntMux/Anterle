#include <stdint.h>

#include "game.hpp"

Game::Game(uint16_t width, uint16_t height)
  : State(GameState::GAME_MENU), Width(width), Height(height), Keys(), KeysProcessed()
{}

void Game::Init() {}
void Game::Update(float dt) {}
void Game::ProcessInput(float dt) {}
void Game::Render() {}
void Game::ResetLevel() {}
