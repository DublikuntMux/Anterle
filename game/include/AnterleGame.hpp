#pragma once

#include <cstdint>
#include <string>

#include "game.hpp"

struct GameConfigs
{
  bool AutoSaveEnable = true;
  uint32_t AutoSavePeriod = 1 * 60 * 60;
  uint16_t CurentLevel = 0;
  uint32_t CurentLine = 0;
  std::string PlayerName = "Player";
};

class AnterleGame : public Anterle::Game
{
public:
  GameConfigs *Configs;

  AnterleGame();
  AnterleGame(GameConfigs *Configs);
};
