#include <memory>

#include "imgui/plot_var.hpp"
#include <loguru.hpp>

#include "backend/vulkan.hpp"
#include "game.hpp"

Game::Game(const char *window_name) : backend(std::make_unique<VulkanBackend>(window_name))
{
  loguru::add_file("logging.log", loguru::Truncate, loguru::Verbosity_INFO);
}
Game::~Game() {}

void Game::Update(double deltaTime)
{
  ImGui::SetNextWindowSize(ImVec2(470, 200));
  {
    ImGui::Begin("Debug window");
    ImGui::PlotVar("Delta time", deltaTime);
    ImGui::End();
  }
}

void Game::Start()
{
  double deltaTime = 0.0;
  double lastFrame = 0.0;
  while (!glfwWindowShouldClose(backend->GetGLFWWindow())) {
    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    backend->PreRender();
    Update(deltaTime);
    backend->PostRender();
  }
}
