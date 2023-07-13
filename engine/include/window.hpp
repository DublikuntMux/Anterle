#pragma once

#include <memory>

#include <GLFW/glfw3.h>
#include <stdint.h>

#include "game.hpp"

class Window
{
public:
  Game *GameInstance;
  GLFWwindow *window;
  bool debug_mode = false;

  uint32_t ScreenWidth;
  uint32_t ScreenHight;

  Window(uint32_t screen_width, uint32_t screen_hight, const char *name, Game *instance);
  virtual ~Window();

  virtual void Init();
  virtual void Start();

private:
  static void glfw_error_callback(int error, const char *description);
  void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
};
