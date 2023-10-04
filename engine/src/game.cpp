#include <memory>

#include <loguru.hpp>

#include "backend/vulkan.hpp"
#include "game.hpp"
#include "imgui/plot_var.hpp"


Game::Game(int width, int height, const char *window_name)
  : debug_mode(false), backend(std::make_unique<VulkanBackend>(width, height, window_name)),
    audio_system(Anterle::AudioSystem())
{
  loguru::add_file("logging.log", loguru::Truncate, loguru::Verbosity_INFO);
}
Game::~Game() {}

void Game::Update(double deltaTime) {}

void Game::Start()
{
  double deltaTime = 0.0;
  double lastFrame = 0.0;

  while (!glfwWindowShouldClose(backend->GetGLFWWindow())) {
    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    backend->PreRender();

    if (debug_mode) {

      ImGui::SetNextWindowSize(ImVec2(470, 200));
      {
        ImGui::Begin("Debug window");
        ImGui::PlotVar("Delta time", deltaTime);
        ImGui::End();
      }
    }
    Update(deltaTime);

    backend->PostRender();
  }
}
