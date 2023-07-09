#include <cstdint>
#include <iostream>
#include <memory>

#include <imgui.h>
#include "imgui/imgui_impl_glfw.hpp"
#include "imgui/imgui_impl_opengl3.hpp"
#include "imgui/plot_var.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

#include "debug/profiler.hpp"
#include "resource/resource_manager.hpp"

#include "AnterleGame.hpp"

const uint32_t SCREEN_WIDTH = 1280;
const uint32_t SCREEN_HEIGHT = 720;
bool debug_mode = false;

std::unique_ptr<AnterleGame> gameInstance;

static void glfw_error_callback(int error, const char *description)
{
  PLOG_WARNING << "GLFW Error " << error << ": " << description;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { glfwSetWindowShouldClose(window, 1); }
  if (key == GLFW_KEY_D && action == GLFW_PRESS) {
    if (debug_mode) {
      debug_mode = false;
      PLOG_INFO << "Disable debuge mode.";
    } else {
      debug_mode = true;
      PLOG_INFO << "Enable debuge mode.";
    }
  }

  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS) {
      gameInstance->Keys[key] = true;
    } else if (action == GLFW_RELEASE) {
      gameInstance->Keys[key] = false;
      gameInstance->KeysProcessed[key] = false;
    }
  }
}

int main(int argc, char *argv[])
{
  plog::init(plog::debug, "loger.log", 5 * 1024 * 1024, 5);
  plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
  plog::get()->addAppender(&consoleAppender);

  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) return 1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, false);

  GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Antele", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  if (window == nullptr) {
    PLOG_ERROR << "Failed to initialize window.";
    return 1;
  }

  glfwSwapInterval(1);
  glfwSetKeyCallback(window, key_callback);

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  gameInstance = std::make_unique<AnterleGame>(SCREEN_WIDTH, SCREEN_HEIGHT);

  gameInstance->Init();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  io.ConfigViewportsNoTaskBarIcon = true;
  io.IniFilename = nullptr;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");

  float deltaTime = 0.0F;
  float lastFrame = 0.0F;
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
    gameInstance->ProcessInput(deltaTime);
    profiler.End(Profiler::Stage::PressInput);

    profiler.Begin(Profiler::Stage::Update);
    gameInstance->Update(deltaTime);
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
        PlotFlame("GPU",
          &ProfilerValueGetter,
          &entry,
          Profiler::_StageCount,
          0,
          "Main Thread",
          FLT_MAX,
          FLT_MAX,
          ImVec2(400, 0));
        ImGui::PlotVar("Delta time", deltaTime);
        ImGui::PlotVar("FPS", io.Framerate);
        ImGui::End();
      }
    }
    profiler.End(Profiler::Stage::ImguiNewFrame);

    profiler.Begin(Profiler::Stage::OpenGL);
    ImGui::Render();
    glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);
    profiler.Begin(Profiler::Stage::GameRender);
    gameInstance->Render();
    profiler.End(Profiler::Stage::GameRender);
    profiler.End(Profiler::Stage::OpenGL);

    profiler.Begin(Profiler::Stage::ImguiRender);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow *backup_current_context = glfwGetCurrentContext();
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