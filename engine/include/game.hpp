#pragma once

#include <array>
#include <cstdint>
#include <memory>

#include <GLFW/glfw3.h>
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

  void saveScreenshot(const char *filename);

  void Start();

  virtual void Init();
  virtual void Update();
  virtual void FixedUpdate();
  virtual void ProcessInput();
  virtual void Render();

public:
  GameState State;
  uint16_t Width, Height;
  const char *Title;

  GLFWwindow *GlfwWindow;

  std::array<bool, 1024> Keys;
  std::array<bool, 1024> KeysProcessed;

  bool debug_mode = false;
  std::shared_ptr<sol::state> Lua;

  std::unique_ptr<Anterle::SpriteRenderer> Renderer;
  std::unique_ptr<Anterle::TextRenderer> Text;
  std::unique_ptr<Anterle::AudioSystem> Audio;

private:
  static void glfw_error_callback(int error, const char *description);
  void mouse_callback(GLFWwindow *window, int button, int action, int modifier);
  void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
};
}// namespace Anterle
