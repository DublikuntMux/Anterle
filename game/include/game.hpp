#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "object/game_level.hpp"
#include "object/game_object.hpp"

enum GameState { GAME_ACTIVE, GAME_MENU, GAME_WIN };

enum Direction { UP, RIGHT, DOWN, LEFT };

struct GameConfigs {
  bool AutoSaveEnable = true;
  uint32_t AutoSavePeriod = 1 * 60 * 60;
  uint16_t CurentLevel = 0;
  uint32_t CurentLine = 0;
  std::string PlayerName = "Player";
};

class Game {
public:
  GameState State;
  GameConfigs Configs;
  uint16_t Width, Height;

  std::vector<GameLevel> Levels;

  bool Keys[1024];
  bool KeysProcessed[1024];

  Game(uint16_t width, uint16_t height);
  ~Game();

  void Init();
  void ProcessInput(float dt);
  void Update(float dt);
  void Render();
  void ResetLevel();
};
