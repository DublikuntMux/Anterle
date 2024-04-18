#include <cstdint>
#include <memory>

#include <glad/glad.h>

#include <SDL.h>
#include <SDL_opengles2.h>

#include <fpng.h>
#include <glm/ext.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

#include <sol/sol.hpp>

#include "SDL_video.h"
#include "debug/profiler.hpp"
#include "game.hpp"
#include "imgui/notify.hpp"
#include "imgui/plot_var.hpp"
#include "logger.hpp"
#include "resource/resource_manager.hpp"
#include "resource/time.hpp"
#include "scripting/glm/vector.hpp"
#include "utils.hpp"

namespace Anterle {
Game::Game(uint16_t width, uint16_t height, const char *title)
  : State(GameState::GAME_MENU), Width(width), Height(height), Title(title)
{
  Logger::getInstance()->log("Starting game.");

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER | SDL_INIT_SENSOR) < 0) {
    Logger::getInstance()->log("Failed to initialize GLFW.");
    abort();
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#ifdef __APPLE__
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

#ifdef SDL_HINT_IME_SHOW_UI
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  Window = SDL_CreateWindow(title,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    Width,
    Height,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
  if (!Window) {
    Logger::getInstance()->log("Failed to create window: %s", SDL_GetError());
    abort();
  }
  glContext = SDL_GL_CreateContext(Window);
  if (!glContext) {
    Logger::getInstance()->log("Failed to create OpenGL context: %s", SDL_GetError());
    abort();
  }

  gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress);

  SDL_GL_MakeCurrent(Window, glContext);
  SDL_GL_SetSwapInterval(1);

  glViewport(0, 0, Width, Height);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = &ImGui::GetIO();
  (void)io;

  io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io->IniFilename = nullptr;

  Utils::SetupImGuiStyle();

  ImGui_ImplSDL2_InitForOpenGL(Window, glContext);
  ImGui_ImplOpenGL3_Init("#version 300 es");
}

Game::~Game()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(Window);
  SDL_Quit();
}

void Game::Start()
{
  Profiler profiler;

  while (!quit) {
    Time::update();
    profiler.Frame();
    profiler.Begin(Profiler::Stage::PollEvents);
    HandleEvents();
    profiler.End(Profiler::Stage::PollEvents);

    profiler.Begin(Profiler::Stage::GameEvents);
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
    ImGui_ImplSDL2_NewFrame();
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
    glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);
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
    SDL_GL_SwapWindow(Window);
    SDL_Delay(1);
    profiler.End(Profiler::Stage::SwapBuffer);
  }
}

void Game::Init()
{
  Logger::getInstance()->log("Init resources.");
  auto &resource = ResourceManager::getInstance();

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

  Logger::getInstance()->log("Load lua scripting.");
  Lua = std::make_shared<sol::state>();
  Lua->open_libraries(sol::lib::base,
    sol::lib::string,
    sol::lib::math,
    sol::lib::io,
    sol::lib::table,
    sol::lib::package,
    sol::lib::utf8,
    sol::lib::count);
  Andterle::Scripting::BindGLMVectors(*Lua);
}

void Game::Update() {}
void Game::FixedUpdate() {}
void Game::ProcessInput(SDL_Keycode _) {}
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

void Game::HandleEvents()
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT) {
      quit = true;
    } else if (event.type == SDL_KEYDOWN) {
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        quit = true;
      } else if (event.key.keysym.sym == SDLK_d) {
        debug_mode = !debug_mode;
        if (debug_mode) {
          ImGui::InsertNotification({ ImGuiToastType::Info, 3000, "Enable debug mode." });
        } else {
          ImGui::InsertNotification({ ImGuiToastType::Info, 3000, "Disable debug mode." });
        }
      }
      ProcessInput(event.key.keysym.sym);
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
      if (event.button.button == SDL_BUTTON_LEFT) {
        int xpos = 0, ypos = 0;
        SDL_GetMouseState(&xpos, &ypos);
      }
    }
  }
}
}// namespace Anterle
