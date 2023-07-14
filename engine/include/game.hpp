#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "object/game_level.hpp"
#include "object/game_object.hpp"

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

  virtual void Init();
  virtual void Update(float dt);
  virtual void ProcessInput(float dt);
  virtual void Render();
  virtual void ResetLevel();
};
