#pragma once

#include <chrono>
#include <string>

#include <imgui.h>

#define NOTIFY_MAX_MSG_LENGTH 4096
#define NOTIFY_PADDING_X 20.f
#define NOTIFY_PADDING_Y 20.f
#define NOTIFY_PADDING_MESSAGE_Y 10.f
#define NOTIFY_FADE_IN_OUT_TIME 150
#define NOTIFY_DEFAULT_DISMISS 3000
#define NOTIFY_OPACITY 1.0f
#define NOTIFY_TOAST_FLAGS                                                                      \
  ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs \
    | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing

#define NOTIFY_NULL_OR_EMPTY(str) (!str || !strlen(str))
#define NOTIFY_FORMAT(fn, format, ...) \
  if (format) {                        \
    va_list args;                      \
    va_start(args, format);            \
    fn(format, args, __VA_ARGS__);     \
    va_end(args);                      \
  }

namespace Anterle {
enum class ImGuiToastType { None, Success, Warning, Error, Info, COUNT };
enum class ImGuiToastPhase { FadeIn, Wait, FadeOut, Expired, COUNT };
enum class ImGuiToastPos { TopLeft, TopCenter, TopRight, BottomLeft, BottomCenter, BottomRight, Center, COUNT };

class ImGuiToast
{
public:
  ImGuiToast(ImGuiToastType type, int dismiss_time = NOTIFY_DEFAULT_DISMISS) : type(type), dismiss_time(dismiss_time)
  {
    creation_time =
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
        .count();
  }
  ImGuiToast(ImGuiToastType type, const char *format, ...) : ImGuiToast(type) { NOTIFY_FORMAT(set_content, format); }
  ImGuiToast(ImGuiToastType type, int dismiss_time, const char *format, ...) : ImGuiToast(type, dismiss_time)
  {
    NOTIFY_FORMAT(set_content, format);
  }

  void set_title(const char *format, ...);
  void set_content(const char *format, ...);
  void set_type(const ImGuiToastType &type);

  const char *get_title();
  const char *get_default_title();
  const ImGuiToastType &get_type();
  ImVec4 get_color();
  const char *get_icon();
  char *get_content();
  int64_t get_elapsed_time();
  ImGuiToastPhase get_phase();
  float get_fade_percent();

private:
  ImGuiToastType type = ImGuiToastType::None;
  std::string title;
  std::string content;
  int dismiss_time = NOTIFY_DEFAULT_DISMISS;
  int64_t creation_time = 0;

  void set_title(const char *format, va_list args);
  void set_content(const char *format, va_list args);
};
}// namespace Anterle

namespace ImGui {
void InsertNotification(const Anterle::ImGuiToast &toast);
void RemoveNotification(int index);
void RenderNotifications();
void MergeIconsWithLatestFont(float font_size, bool FontDataOwnedByAtlas = false);
}// namespace ImGui
