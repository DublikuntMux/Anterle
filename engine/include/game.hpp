#pragma once

#include <memory>

#include "audio/audio_system.hpp"
#include "object/scene.hpp"
#include "renderer/backend.hpp"

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
  std::unique_ptr<VulkanBackend> backend;
  Anterle::AudioSystem audio_system;
};
