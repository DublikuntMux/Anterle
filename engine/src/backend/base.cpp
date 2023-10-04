#include "backend/base.hpp"
#include "helper/error.hpp"

BaseBackend::BaseBackend(int width, int height, const char *window_name) : clear_color(0.45f, 0.55f, 0.60f, 1.00f)
{
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) { printf("GLFW: GLFW start failed\n"); }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  window = glfwCreateWindow(width, height, window_name, nullptr, nullptr);
}

BaseBackend::~BaseBackend()
{
  glfwDestroyWindow(window);
  glfwTerminate();
}

GLFWwindow *BaseBackend::GetGLFWWindow() { return window; }

void BaseBackend::PreRender() {}
void BaseBackend::PostRender() {}
