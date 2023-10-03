#include <glad/gles2.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define IMGUI_IMPL_OPENGL_ES3
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <loguru.hpp>

#include "debug/profiler.hpp"
#include "game.hpp"
#include "imgui/plot_var.hpp"
#include "object/ui/notify.hpp"
#include "resource/resource_manager.hpp"
#include "window.hpp"

namespace Anterle {
void SetupImGuiStyle()
{
  ImGuiStyle &style = ImGui::GetStyle();

  style.Alpha = 1.0f;
  style.DisabledAlpha = 0.6000000238418579f;
  style.WindowPadding = ImVec2(8.0f, 8.0f);
  style.WindowRounding = 4.0f;
  style.WindowBorderSize = 0.0f;
  style.WindowMinSize = ImVec2(32.0f, 32.0f);
  style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
  style.WindowMenuButtonPosition = ImGuiDir_Left;
  style.ChildRounding = 0.0f;
  style.ChildBorderSize = 1.0f;
  style.PopupRounding = 4.0f;
  style.PopupBorderSize = 1.0f;
  style.FramePadding = ImVec2(4.0f, 3.0f);
  style.FrameRounding = 2.5f;
  style.FrameBorderSize = 0.0f;
  style.ItemSpacing = ImVec2(8.0f, 4.0f);
  style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
  style.CellPadding = ImVec2(4.0f, 2.0f);
  style.IndentSpacing = 21.0f;
  style.ColumnsMinSpacing = 6.0f;
  style.ScrollbarSize = 11.0f;
  style.ScrollbarRounding = 2.5f;
  style.GrabMinSize = 10.0f;
  style.GrabRounding = 2.0f;
  style.TabRounding = 3.5f;
  style.TabBorderSize = 0.0f;
  style.TabMinWidthForCloseButton = 0.0f;
  style.ColorButtonPosition = ImGuiDir_Right;
  style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
  style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

  style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
  style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.592f, 0.592f, 0.592f, 1.0f);
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.145f, 0.145f, 0.149f, 1.0f);
  style.Colors[ImGuiCol_ChildBg] = ImVec4(0.145f, 0.145f, 0.149f, 1.0f);
  style.Colors[ImGuiCol_PopupBg] = ImVec4(0.145f, 0.145f, 0.149f, 1.0f);
  style.Colors[ImGuiCol_Border] = ImVec4(0.306f, 0.306f, 0.306f, 1.0f);
  style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.306f, 0.306f, 0.306f, 1.0f);
  style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.216f, 1.0f);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.114f, 0.592f, 0.925f, 1.0f);
  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.0f, 0.467f, 0.784f, 1.0f);
  style.Colors[ImGuiCol_TitleBg] = ImVec4(0.145f, 0.145f, 0.149f, 1.0f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.145f, 0.145f, 0.149f, 1.0f);
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.145f, 0.145f, 0.149f, 1.0f);
  style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2f, 0.2f, 0.216f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2f, 0.2f, 0.216f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.322f, 0.322f, 0.333f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.353f, 0.353f, 0.4f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.353f, 0.353f, 0.4f, 1.0f);
  style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 0.467f, 0.784f, 1.0f);
  style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.114f, 0.592f, 0.925f, 1.0f);
  style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.0f, 0.467f, 0.784f, 1.0f);
  style.Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.216f, 1.0f);
  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.114f, 0.592f, 0.925f, 1.0f);
  style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.114f, 0.592f, 0.925f, 1.0f);
  style.Colors[ImGuiCol_Header] = ImVec4(0.2f, 0.2f, 0.216f, 1.0f);
  style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.114f, 0.592f, 0.925f, 1.0f);
  style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0f, 0.467f, 0.784f, 1.0f);
  style.Colors[ImGuiCol_Separator] = ImVec4(0.306f, 0.306f, 0.306f, 1.0f);
  style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.306f, 0.306f, 0.306f, 1.0f);
  style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.306f, 0.306f, 0.306f, 1.0f);
  style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.145f, 0.145f, 0.149f, 1.0f);
  style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2f, 0.2f, 0.216f, 1.0f);
  style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.322f, 0.322f, 0.333f, 1.0f);
  style.Colors[ImGuiCol_Tab] = ImVec4(0.145f, 0.145f, 0.149f, 1.0f);
  style.Colors[ImGuiCol_TabHovered] = ImVec4(0.114f, 0.592f, 0.925f, 1.0f);
  style.Colors[ImGuiCol_TabActive] = ImVec4(0.0f, 0.467f, 0.784f, 1.0f);
  style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.145f, 0.145f, 0.149f, 1.0f);
  style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0f, 0.467f, 0.784f, 1.0f);
  style.Colors[ImGuiCol_PlotLines] = ImVec4(0.0f, 0.467f, 0.784f, 1.0f);
  style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.114f, 0.592f, 0.925f, 1.0f);
  style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0f, 0.467f, 0.784f, 1.0f);
  style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.114f, 0.592f, 0.925f, 1.0f);
  style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.188f, 0.188f, 0.2f, 1.0f);
  style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.309f, 0.309f, 0.349f, 1.0f);
  style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.227f, 0.227f, 0.2475f, 1.0f);
  style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
  style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
  style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.0f, 0.467f, 0.784f, 1.0f);
  style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.145f, 0.145f, 0.149f, 1.0f);
  style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.145f, 0.145f, 0.149f, 1.0f);
  style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
  style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
  style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.145f, 0.145f, 0.149f, 1.0f);
}

void Window::glfw_error_callback(int error, const char *description)
{
  LOG_F(ERROR, "GLFW Error: %d : %s", error, description);
}

void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { glfwSetWindowShouldClose(window, GL_TRUE); }
  if (key == GLFW_KEY_D && action == GLFW_PRESS) {
    if (debug_mode) {
      debug_mode = false;
      ImGui::InsertNotification({ ImGuiToastType::Info, 3000, "Disable debuge mode." });
    } else {
      debug_mode = true;
      ImGui::InsertNotification({ ImGuiToastType::Info, 3000, "Enable debuge mode." });
    }
  }

  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS) {
      GameInstance->Keys[key] = true;
    } else if (action == GLFW_RELEASE) {
      GameInstance->Keys[key] = false;
      GameInstance->KeysProcessed[key] = false;
    }
  }
}

void Window::mouse_callback(GLFWwindow *window, int button, int action, int modifier)
{
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1) { this->CheckClickables(xpos, ypos); }
}

Window::Window(uint32_t screen_width, uint32_t screen_hight, const char *name, Game *instance)
  : ScreenWidth(screen_width), ScreenHight(screen_hight), GameInstance(instance)
{
  loguru::add_file("logging.log", loguru::Truncate, loguru::Verbosity_INFO);

  glfwSetErrorCallback(&Window::glfw_error_callback);
  if (!glfwInit()) { ABORT_F("Failed to initialize GLFW."); }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  this->window = glfwCreateWindow(screen_width, screen_hight, name, nullptr, nullptr);
  glfwMakeContextCurrent(this->window);
  if (this->window == nullptr) { ABORT_F("Failed to initialize window."); }

  gladLoadGLES2(glfwGetProcAddress);
}

Window::~Window()
{
  ResourceManager::Clear();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}

void Window::Init()
{
  glfwSwapInterval(1);
  glfwSetWindowUserPointer(window, this);
  glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mode) {
    auto &self = *static_cast<Window *>(glfwGetWindowUserPointer(window));
    self.key_callback(window, key, scancode, action, mode);
  });

  glViewport(0, 0, this->ScreenWidth, this->ScreenHight);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.IniFilename = nullptr;

  SetupImGuiStyle();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 300 es");

  this->GameInstance->Init();
}

void Window::Start()
{
  double deltaTime = 0.0;
  double lastFrame = 0.0;
  Profiler profiler;

  while (!glfwWindowShouldClose(window)) {
    profiler.Frame();

    profiler.Begin(Profiler::Stage::PollEvents);
    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    glfwPollEvents();
    profiler.End(Profiler::Stage::PollEvents);

    profiler.Begin(Profiler::Stage::GameEvents);
    profiler.Begin(Profiler::Stage::PressInput);
    this->GameInstance->ProcessInput(deltaTime);
    profiler.End(Profiler::Stage::PressInput);

    profiler.Begin(Profiler::Stage::Update);
    this->GameInstance->Update(deltaTime);
    profiler.End(Profiler::Stage::Update);
    profiler.End(Profiler::Stage::GameEvents);

    profiler.Begin(Profiler::Stage::ImguiNewFrame);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (debug_mode) {
      ImGui::SetNextWindowSize(ImVec2(470, 200));
      {
        ImGui::Begin("Debug window");
        auto &entry = profiler.entries[profiler.GetCurrentEntryIndex()];
        PlotFlame("GPU",
          &ProfilerValueGetter,
          &entry,
          Profiler::_StageCount,
          0,
          "Main Thread",
          FLT_MAX,
          FLT_MAX,
          ImVec2(400, 0));
        ImGui::PlotVar("Delta time", deltaTime);
        ImGui::End();
      }
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
    ImGui::RenderNotifications();
    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(1);
    profiler.End(Profiler::Stage::ImguiNewFrame);

    profiler.Begin(Profiler::Stage::OpenGL);
    ImGui::Render();
    glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);
    profiler.Begin(Profiler::Stage::GameRender);
    this->GameInstance->Render();
    profiler.End(Profiler::Stage::GameRender);
    profiler.End(Profiler::Stage::OpenGL);

    profiler.Begin(Profiler::Stage::ImguiRender);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    profiler.End(Profiler::Stage::ImguiRender);

    profiler.Begin(Profiler::Stage::SwapBuffer);
    glfwSwapBuffers(window);
    profiler.End(Profiler::Stage::SwapBuffer);
  }
}

void Window::CheckClickables(uint32_t x, uint32_t y)
{
  for (Clickable clickable : clickables) { clickable.CheckClick(x, y); }
}
}// namespace Anterle
