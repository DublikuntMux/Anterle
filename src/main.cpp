#include <cstdint>
#include <iostream>

#include <imgui.h>
#include "imgui/imgui_impl_glfw.hpp"
#include "imgui/imgui_impl_opengl3.hpp"
#include "imgui/plot_var.hpp"

#include <spdlog/spdlog.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "game.hpp"
#include "debug/profiler.hpp"
#include "resource/resource_manager.hpp"

const uint32_t SCREEN_WIDTH = 800;
const uint32_t SCREEN_HEIGHT = 600;
bool debug_mode = false;

Game gameInstance(SCREEN_WIDTH, SCREEN_HEIGHT);

static void glfw_error_callback(int error, const char *description) {
  spdlog::warn("GLFW Error {0}: {1}", error, description);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (key == GLFW_KEY_D && action == GLFW_PRESS) {
    if (debug_mode) {
      debug_mode = false;
      spdlog::info("Disable debuge mode.");
    } else {
      debug_mode = true;
      spdlog::info("Enable debuge mode.");
    }
  }

  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS)
      gameInstance.Keys[key] = true;
    else if (action == GLFW_RELEASE) {
      gameInstance.Keys[key] = false;
      gameInstance.KeysProcessed[key] = false;
    }
  }
}

int main(int argc, char *argv[]) {
  spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v");

  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, false);

  GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Antele",
                                        nullptr, nullptr);
  glfwMakeContextCurrent(window);
  if (window == nullptr)
    return 1;

  glfwSwapInterval(1);
  glfwSetKeyCallback(window, key_callback);

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  gameInstance.Init();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  io.ConfigViewportsNoTaskBarIcon = true;
  io.IniFilename = NULL;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
  Profiler profiler;

  while (!glfwWindowShouldClose(window)) {
    profiler.Frame();

    profiler.Begin(Profiler::Stage::PollEvents);
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    glfwPollEvents();
    profiler.End(Profiler::Stage::PollEvents);

    profiler.Begin(Profiler::Stage::GameEvents);
    profiler.Begin(Profiler::Stage::PressInput);
    gameInstance.ProcessInput(deltaTime);
    profiler.End(Profiler::Stage::PressInput);

    profiler.Begin(Profiler::Stage::Update);
    gameInstance.Update(deltaTime);
    profiler.End(Profiler::Stage::Update);
    profiler.End(Profiler::Stage::GameEvents);

    profiler.Begin(Profiler::Stage::ImguiNewFrame);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (debug_mode) {
      ImGui::SetNextWindowSize(ImVec2(470, 200));
      {
        ImGui::Begin("Debug window");
        auto &entry = profiler.entries[profiler.GetCurrentEntryIndex()];
        PlotFlame("CPU", &ProfilerValueGetter, &entry, Profiler::_StageCount, 0,
                  "Main Thread", FLT_MAX, FLT_MAX, ImVec2(400, 0));
        ImGui::PlotVar("Delta time", deltaTime);
        ImGui::PlotVar("FPS", io.Framerate);
        ImGui::End();
      }
    }
    profiler.End(Profiler::Stage::ImguiNewFrame);

    profiler.Begin(Profiler::Stage::OpenGL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    profiler.Begin(Profiler::Stage::GameRender);
    gameInstance.Render();
    profiler.End(Profiler::Stage::GameRender);
    profiler.End(Profiler::Stage::OpenGL);

    profiler.Begin(Profiler::Stage::ImguiRender);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
    profiler.End(Profiler::Stage::ImguiRender);

    profiler.Begin(Profiler::Stage::SwapBuffer);
    glfwSwapBuffers(window);
    profiler.End(Profiler::Stage::SwapBuffer);
  }
  ResourceManager::Clear();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}