#pragma once

#include <anterle_engine.hpp>

struct GameConfigs
{
  bool AutoSaveEnable = true;
  uint32_t AutoSavePeriod = 1 * 60 * 60;
  uint16_t CurentLevel = 0;
  uint32_t CurentLine = 0;
  std::string PlayerName = "Player";

  GameConfigs() = default;

  GameConfigs(bool AutoSaveEnable,
    const uint32_t &AutoSavePeriod,
    const uint16_t &CurentLevel,
    const uint32_t &CurentLine,
    const std::string &PlayerName)
    : AutoSaveEnable(AutoSaveEnable), AutoSavePeriod(AutoSavePeriod), CurentLevel(CurentLevel), CurentLine(CurentLine),
      PlayerName(PlayerName)
  {}

  bool operator==(const GameConfigs &other) const
  {
    return AutoSaveEnable == other.AutoSaveEnable && AutoSavePeriod == other.AutoSavePeriod
           && CurentLevel == other.CurentLevel && CurentLine == other.CurentLine && PlayerName == other.PlayerName;
  }
};

class AnterleGame : public Game
{
public:
  GameConfigs Configs;

  AnterleGame(uint16_t width, uint16_t height);
  AnterleGame(uint16_t width, uint16_t height, const GameConfigs &Configs);

  void Init();
  void Update(float dt);
  void ProcessInput(float dt);
  void Render();
  void ResetLevel();
};
