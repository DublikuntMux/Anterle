#pragma once

#include <stdint.h>
#include <vector>

#include <GLFW/glfw3.h>

#include "game.hpp"
#include "object/ui/button.hpp"

class Window
{
public:
  Game *GameInstance;
  GLFWwindow *window;

  std::vector<Button> buttons;
  bool debug_mode = false;

  uint32_t ScreenWidth;
  uint32_t ScreenHight;

  Window(uint32_t screen_width, uint32_t screen_hight, const char *name, Game *instance);
  virtual ~Window();

  virtual void Init();
  virtual void Start();
  void CheckButtons(uint64_t x, uint64_t y);

private:
  static void glfw_error_callback(int error, const char *description);
  void mouse_callback(GLFWwindow* window, int button, int action, int modifier);
  void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
};
