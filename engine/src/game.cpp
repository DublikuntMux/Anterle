#include <cmath>
#include <cstdint>

#include <GLFW/glfw3.h>
#include <fpng.h>
#include <glad/glad.h>
#include <glm/ext.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "debug/profiler.hpp"
#include "game.hpp"
#include "imgui/notify.hpp"
#include "imgui/plot_var.hpp"
#include "logger.hpp"
#include "resource/resource_manager.hpp"
#include "resource/time.hpp"
#include "utils.hpp"

namespace Anterle {
Game::Game(uint16_t width, uint16_t height, const char *title)
  : State(GameState::GAME_MENU), Width(width), Height(height), Title(title), Keys(), KeysProcessed()
{
  Logger::getInstance()->log("Starting game.");
  glfwSetErrorCallback(&Game::glfw_error_callback);

  if (!glfwInit()) {
    Logger::getInstance()->log("Failed to initialize GLFW.");
    abort();
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);
  int screenWidth = mode->width;
  int screenHeight = mode->height;

  GlfwWindow = glfwCreateWindow(Width, Height, title, nullptr, nullptr);
  glfwSetWindowPos(GlfwWindow, (screenWidth - Width) / 2, (screenHeight - Height) / 2);
  glfwMakeContextCurrent(GlfwWindow);
  if (GlfwWindow == nullptr) {
    Logger::getInstance()->log("Failed to initialize window.");
    abort();
  }

  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

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
  glEnable(GL_CULL_FACE);
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
  ImGui_ImplOpenGL3_Init("#version 130");
}

Game::~Game()
{
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
        auto &entry = profiler.getEntry(profiler.GetCurrentEntryIndex());
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
    glClearColor(0.3f, 0.2f, 0.6f, 1.0f);
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

void Game::Init()
{
Logger::getInstance()->log("Init resources.");
  auto &resource = ResourceManager::getInstance();
  resource->LoadShader("sprite");

  glm::mat4 projection = glm::ortho(0.0F, static_cast<float>(Width), static_cast<float>(Height), 0.0F, -1.0F, 1.0F);

  resource->GetShader("sprite").Use().SetInteger("sprite", 0);
  resource->GetShader("sprite").SetMatrix4("projection", projection);

  Logger::getInstance()->log("Load renderer.");
  Renderer = std::make_unique<Anterle::SpriteRenderer>(resource->GetShader("sprite"));

  Logger::getInstance()->log("Load text rendering.");
  Text = std::make_unique<Anterle::TextRenderer>(Width, Height);
  Text->Load("tahoma");
  Logger::getInstance()->log("Load audio system.");
  Audio = std::make_unique<Anterle::AudioSystem>();
}

void Game::Update() {}
void Game::FixedUpdate() {}
void Game::ProcessInput() {}
void Game::Render() {}

void Game::saveScreenshot(const char *filename)
{
  Logger::getInstance()->log("Screenshot saved to %s", filename);
  std::vector<GLubyte> pixels(3 * Width * Height);

  glReadPixels(0, 0, Width, Height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

  std::vector<GLubyte> flippedPixels(3 * Width * Height);
  for (int y = 0; y < Height; ++y) {
    for (int x = 0; x < Width; ++x) {
      for (int c = 0; c < 3; ++c) {
        flippedPixels[3 * (y * Width + x) + c] = pixels[3 * ((Height - y - 1) * Width + x) + c];
      }
    }
  }

  if (!fpng::fpng_encode_image_to_file(filename, flippedPixels.data(), Width, Height, 3, fpng::FPNG_ENCODE_SLOWER)) {}
}

void Game::glfw_error_callback(int error, const char *description)
{
  Logger::getInstance()->log("GLFW Error: %d : %s", error, description);
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
      Keys.at(key) = true;
    } else if (action == GLFW_RELEASE) {
      Keys.at(key) = false;
      KeysProcessed.at(key) = false;
    }
  }
}

void Game::mouse_callback(GLFWwindow *window, int button, int action, int)
{
  double xpos = NAN, ypos = NAN;
  glfwGetCursorPos(window, &xpos, &ypos);
  if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1) {}
}
}// namespace Anterle
