#pragma once

#include <cstdint>
#include <vector>

#include "object/game_level.hpp"

namespace Anterle {
enum class GameState { GAME_ACTIVE, GAME_MENU, GAME_WIN };

class Game
{
public:
  GameState State;
  uint16_t Width, Height;

  std::vector<GameLevel> Levels;

  bool Keys[1024];
  bool KeysProcessed[1024];

  Game(uint16_t width, uint16_t height);
  virtual ~Game();

  virtual void Init();
  virtual void Update();
  virtual void FixedUpdate();
  virtual void ProcessInput();

  virtual void Render();

  virtual void ResetLevel();
};
}// namespace Anterle
