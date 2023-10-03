#include <stdint.h>

#include "game.hpp"

namespace Anterle {
Game::Game(uint16_t width, uint16_t height)
  : State(GameState::GAME_MENU), Width(width), Height(height), Keys(), KeysProcessed()
{}

Game::~Game(){
  delete Renderer;
  delete Particles;
  delete Text;
  delete Audio;
}

void Game::Init() {}
void Game::Update(double dt) {}
void Game::ProcessInput(double dt) {}
void Game::Render() {}
void Game::ResetLevel() {}
}// namespace Anterle
