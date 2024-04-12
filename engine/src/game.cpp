#include <cstdint>

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#define IMGUI_IMPL_OPENGL_ES3
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <fpng.h>
#include <loguru.hpp>

#include "debug/profiler.hpp"
#include "game.hpp"
#include "imgui/plot_var.hpp"
#include "object/ui/notify.hpp"
#include "resource/resource_manager.hpp"
#include "resource/time.hpp"
#include "utils.hpp"

namespace Anterle {
Game::Game(uint16_t width, uint16_t height, const char *title)
  : State(GameState::GAME_MENU), Width(width), Height(height), Title(title), Keys(), KeysProcessed()
{
  loguru::add_file("logging.log", loguru::Truncate, loguru::Verbosity_INFO);

  glfwSetErrorCallback(&Game::glfw_error_callback);
  if (!glfwInit()) { ABORT_F("Failed to initialize GLFW."); }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);
  int screenWidth = mode->width;
  int screenHeight = mode->height;

  GlfwWindow = glfwCreateWindow(Width, Height, title, nullptr, nullptr);
  glfwSetWindowPos(GlfwWindow, (screenWidth - Width) / 2, (screenHeight - Height) / 2);
  glfwMakeContextCurrent(GlfwWindow);
  if (GlfwWindow == nullptr) { ABORT_F("Failed to initialize window."); }

  gladLoadGL(glfwGetProcAddress);

  glfwSwapInterval(1);
  glfwSetWindowUserPointer(GlfwWindow, this);
  glfwSetKeyCallback(GlfwWindow, [](GLFWwindow *window, int key, int scancode, int action, int mode) {
    auto &self = *static_cast<Game *>(glfwGetWindowUserPointer(window));
    self.key_callback(window, key, scancode, action, mode);
  });
  glfwSetMouseButtonCallback(GlfwWindow, [](GLFWwindow *window, int button, int action, int mods) {
    auto &self = *static_cast<Game *>(glfwGetWindowUserPointer(window));
    self.mouse_callback(window, button, action, mods);
  });

  glViewport(0, 0, Width, Height);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.IniFilename = nullptr;

  SetupImGuiStyle();

  ImGui_ImplGlfw_InitForOpenGL(GlfwWindow, true);
  ImGui_ImplOpenGL3_Init("#version 460 core");
}

Game::~Game()
{
  ResourceManager::Clear();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(GlfwWindow);
  glfwTerminate();
}

void Game::Start()
{
  Profiler profiler;

  while (!glfwWindowShouldClose(GlfwWindow)) {
    profiler.Frame();
    Time::update();
    profiler.Begin(Profiler::Stage::PollEvents);
    glfwPollEvents();
    profiler.End(Profiler::Stage::PollEvents);

    profiler.Begin(Profiler::Stage::GameEvents);
    profiler.Begin(Profiler::Stage::PressInput);
    ProcessInput();
    profiler.End(Profiler::Stage::PressInput);

    profiler.Begin(Profiler::Stage::FixedUpdate);
    while (Time::FixedAccumulator >= Time::FixedTimeStep) {
      FixedUpdate();
      Time::FixedAccumulator -= Time::FixedTimeStep;
    }
    profiler.End(Profiler::Stage::FixedUpdate);

    profiler.Begin(Profiler::Stage::Update);
    Update();
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
        ImGui::PlotVar("Delta time", static_cast<float>(Time::getDeltaTime()));
        ImGui::End();
      }
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
    ImGui::RenderNotifications();
    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(1);
    profiler.End(Profiler::Stage::ImguiNewFrame);

    profiler.Begin(Profiler::Stage::OpenGL);
    ImGui::Render();
    glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);
    profiler.Begin(Profiler::Stage::GameRender);
    Render();
    profiler.End(Profiler::Stage::GameRender);
    profiler.End(Profiler::Stage::OpenGL);

    profiler.Begin(Profiler::Stage::ImguiRender);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    profiler.End(Profiler::Stage::ImguiRender);

    profiler.Begin(Profiler::Stage::SwapBuffer);
    glfwSwapBuffers(GlfwWindow);
    profiler.End(Profiler::Stage::SwapBuffer);
  }
}

void Game::Init() {}
void Game::Update() {}
void Game::FixedUpdate() {}
void Game::ProcessInput() {}
void Game::Render() {}

void Game::ResetLevel() {}

void Game::saveScreenshot(const char *filename)
{
  GLubyte *pixels = new GLubyte[3 * Width * Height];

  glReadPixels(0, 0, Width, Height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

  GLubyte *flippedPixels = new GLubyte[3 * Width * Height];
  for (int y = 0; y < Height; ++y) {
    for (int x = 0; x < Width; ++x) {
      for (int c = 0; c < 3; ++c) {
        flippedPixels[3 * (y * Width + x) + c] = pixels[3 * ((Height - y - 1) * Width + x) + c];
      }
    }
  }

  if (!fpng::fpng_encode_image_to_file(filename, flippedPixels, Width, Height, 3, fpng::FPNG_ENCODE_SLOWER)) {}

  delete[] pixels;
  delete[] flippedPixels;
}

void Game::glfw_error_callback(int error, const char *description)
{
  LOG_F(ERROR, "GLFW Error: %d : %s", error, description);
}

void Game::key_callback(GLFWwindow *window, int key, int, int action, int)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { glfwSetWindowShouldClose(window, GL_TRUE); }
  if (key == GLFW_KEY_D && action == GLFW_PRESS) {
    if (debug_mode) {
      debug_mode = false;
      ImGui::InsertNotification({ ImGuiToastType::Info, 3000, "Disable debuge mode." });
    } else {
      debug_mode = true;
      ImGui::InsertNotification({ ImGuiToastType::Info, 3000, "Enable debuge mode." });
    }
  }

  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS) {
      Keys[key] = true;
    } else if (action == GLFW_RELEASE) {
      Keys[key] = false;
      KeysProcessed[key] = false;
    }
  }
}

void Game::mouse_callback(GLFWwindow *window, int button, int action, int)
{
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1) { CheckClickables(xpos, ypos); }
}

void Game::CheckClickables(double x, double y)
{
  for (Clickable clickable : clickables) { clickable.CheckClick(x, y); }
}
}// namespace Anterle
