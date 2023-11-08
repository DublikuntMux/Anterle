#pragma once

#include <vector>

#include <GLFW/glfw3.h>

#include "game.hpp"
#include "object/ui/clickable.hpp"

namespace Anterle {
class Window
{
public:
  Game *GameInstance;
  GLFWwindow *GlfwWindow;

  std::vector<Clickable> clickables;
  bool debug_mode = false;

  int WindowWidth;
  int WindowHight;
  const char *WindowName;

  bool showClose = true;

  Window(int window_width, int window_hight, const char *name, Game *instance, bool show_close = true);
  virtual ~Window();

  virtual void Start();

private:
  double _lastMouseX = 0;
  double _lastMouseY = 0;
  bool _isDragging = false;

  void CheckClickables(double x, double y);

  static void glfw_error_callback(int error, const char *description);
  void mouse_callback(GLFWwindow *window, int button, int action, int modifier);
  void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
  void cursor_callback(GLFWwindow *window, double xpos, double ypos);
};
}// namespace Anterle
