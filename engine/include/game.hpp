#pragma once

#include <memory>

#include "audio/audio_system.hpp"
#include "backend/base.hpp"
#include "object/scene.hpp"

class Game
{
public:
  Game(int width, int height, const char *window_name);
  virtual ~Game();

  virtual void Update(double deltaTime);
  void Start();

  bool debug_mode;
  Scene curent_scene;

private:
  std::unique_ptr<BaseBackend> backend;
  Anterle::AudioSystem audio_system;
};
