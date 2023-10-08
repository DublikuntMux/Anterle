#include "backend/base.hpp"

namespace Anterle {
BaseBackend::BaseBackend(int width, int height, const char *window_name)
{
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window = glfwCreateWindow(width, height, window_name, nullptr, nullptr);

  glfwSetWindowUserPointer(window, this);
}

BaseBackend::~BaseBackend()
{
  glfwDestroyWindow(window);
  glfwTerminate();
}

GLFWwindow *BaseBackend::GetGLFWWindow() { return window; }

void BaseBackend::PreRender() { glfwPollEvents(); }

void BaseBackend::PostRender() {}
}// namespace Anterle
