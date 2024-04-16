#pragma once

#include <cstdint>
#include <string>

#include <anterle_engine.hpp>

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

  AnterleGame(uint16_t width, uint16_t height, const char *title);
  AnterleGame(uint16_t width, uint16_t height, const char *title, GameConfigs *Configs);

  ~AnterleGame();

  void Init();
  void Update();
  void FixedUpdate();
  void ProcessInput();
  void Render();

private:
  Anterle::SpriteRenderer *Renderer;
  Anterle::TextRenderer *Text;
  Anterle::AudioSystem *Audio;
};
