#pragma once

#include <GLFW/glfw3.h>

namespace Anterle {
class BaseBackend
{
public:
  BaseBackend(int width, int height, const char *window_name);
  virtual ~BaseBackend();

  GLFWwindow *GetGLFWWindow();

  void PreRender();
  void PostRender();

protected:
  GLFWwindow *window;
};
}// namespace Anterle
