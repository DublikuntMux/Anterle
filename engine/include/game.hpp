#pragma once

#include <cstdint>
#include <vector>

#include <GLFW/glfw3.h>

#include "object/game_level.hpp"
#include "object/ui/clickable.hpp"

namespace Anterle {
enum class GameState { GAME_ACTIVE, GAME_MENU, GAME_WIN };

class Game
{
public:
  Game(uint16_t width, uint16_t height, const char *title);
  virtual ~Game();

  void saveScreenshot(const char *filename);

  void Start();

  virtual void Init();
  virtual void Update();
  virtual void FixedUpdate();
  virtual void ProcessInput();
  virtual void Render();

  virtual void ResetLevel();

public:
  GameState State;
  uint16_t Width, Height;
  const char *Title;

  GLFWwindow *GlfwWindow;

  std::vector<Clickable> clickables;
  std::vector<GameLevel> Levels;

  bool Keys[1024];
  bool KeysProcessed[1024];

  bool debug_mode = false;

private:
  void CheckClickables(double x, double y);

  static void glfw_error_callback(int error, const char *description);
  void mouse_callback(GLFWwindow *window, int button, int action, int modifier);
  void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
};
}// namespace Anterle
