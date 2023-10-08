#pragma once

#include "audio/audio_system.hpp"
#include "backend/base.hpp"
#include "object/scene.hpp"

namespace Anterle {
class Game
{
public:
  Game(BaseBackend backend);
  virtual ~Game();

  virtual void Update(double deltaTime);
  void Start();

  bool debug_mode;
  Scene curent_scene;

private:
  BaseBackend backend;
  std::unique_ptr<Anterle::AudioSystem> audio_system;
};
}// namespace Anterle
