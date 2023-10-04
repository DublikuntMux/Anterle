#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class BaseBackend
{
public:
  BaseBackend(int width, int height, const char *window_name);
  virtual ~BaseBackend();

  GLFWwindow *GetGLFWWindow();

  virtual void PreRender();
  virtual void PostRender();

protected:
  GLFWwindow *window;
  glm::vec4 clear_color;
};