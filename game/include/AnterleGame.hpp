#pragma once

#include <anterle_engine.hpp>

struct GameConfigs
{
  bool AutoSaveEnable = true;
  uint32_t AutoSavePeriod = 1 * 60 * 60;
  uint16_t CurentLevel = 0;
  uint32_t CurentLine = 0;
  std::string PlayerName = "Player";
};

class AnterleGame : public Game
{
public:
  GameConfigs Configs;

  AnterleGame(uint16_t width, uint16_t height);

  void Init();
  void ProcessInput(float dt);
  void Update(float dt);
  void Render();
  void ResetLevel();
};
