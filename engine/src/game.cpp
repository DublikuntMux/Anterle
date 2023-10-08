#include <GLFW/glfw3.h>

#include <loguru.hpp>

#include "game.hpp"

namespace Anterle {
Game::Game(BaseBackend backend)
  : debug_mode(false), backend(backend),
    audio_system(std::make_unique<AudioSystem>())
{
  loguru::add_file("logging.log", loguru::Truncate, loguru::Verbosity_INFO);
}
Game::~Game() {}

void Game::Update(double deltaTime) { curent_scene.Update(deltaTime); }

void Game::Start()
{
  double deltaTime = 0.0;
  double lastFrame = 0.0;

  while (!glfwWindowShouldClose(backend.GetGLFWWindow())) {
    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    backend.PreRender();
    audio_system->update();
    Update(deltaTime);
    backend.PostRender();
  }
}
}// namespace Anterle
