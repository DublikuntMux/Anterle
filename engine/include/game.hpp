#pragma once

#include <cstdint>
#include <memory>

#include <SDL.h>
#include <imgui.h>
#include <sol/sol.hpp>

#include "audio/auidio_server.hpp"
#include "render/sprite_renderer.hpp"
#include "render/text_renderer.hpp"

namespace Anterle {
enum class GameState { GAME_ACTIVE, GAME_MENU, GAME_WIN };

class Game
{
public:
  Game(uint16_t width, uint16_t height, const char *title);
  virtual ~Game();

  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;
  Game(Game &&) noexcept = delete;
  Game &operator=(Game &&) noexcept = delete;

  void saveScreenshot();

  void Start();

  virtual void Init();
  virtual void Update();
  virtual void FixedUpdate();
  virtual void ProcessInput(SDL_Keycode key);
  virtual void Render();

public:
  GameState State;
  uint16_t Width, Height;
  const char *Title;

  SDL_Window *Window;
  SDL_GLContext glContext;
  ImGuiIO *io;

  bool quit = false;
  bool debugMode = false;
  std::string saveLoaction;

  std::shared_ptr<sol::state> Lua;

  std::unique_ptr<Anterle::SpriteRenderer> Renderer;
  std::unique_ptr<Anterle::TextRenderer> Text;
  std::unique_ptr<Anterle::AudioSystem> Audio;

private:
  void HandleEvents();
};
}// namespace Anterle
