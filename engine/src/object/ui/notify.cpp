#include <imgui.h>

#include "object/ui/IconsFontAwesome6.hpp"
#include "object/ui/notify.hpp"

namespace Anterle {
void ImGuiToast::set_title(const char *format, ...) { NOTIFY_FORMAT(set_title, format); }
void ImGuiToast::set_content(const char *format, ...) { NOTIFY_FORMAT(set_content, format); }
void ImGuiToast::set_type(const ImGuiToastType &tost_type) { type = tost_type; };

char *ImGuiToast::get_title() { return title; };
const char *ImGuiToast::get_default_title()
{
  if (!strlen(title)) {
    switch (type) {
    case ImGuiToastType::COUNT:
    case ImGuiToastType::None:
      return NULL;
    case ImGuiToastType::Success:
      return "Success";
    case ImGuiToastType::Warning:
      return "Warning";
    case ImGuiToastType::Error:
      return "Error";
    case ImGuiToastType::Info:
      return "Info";
    }
  }

  return title;
};
const ImGuiToastType &ImGuiToast::get_type() { return type; };
ImVec4 ImGuiToast::get_color()
{
  switch (type) {
  case ImGuiToastType::COUNT:
  case ImGuiToastType::None:
    return { 255, 255, 255, 255 };
  case ImGuiToastType::Success:
    return { 0, 255, 0, 255 };
  case ImGuiToastType::Warning:
    return { 255, 255, 0, 255 };
  case ImGuiToastType::Error:
    return { 255, 0, 0, 255 };
  case ImGuiToastType::Info:
    return { 0, 157, 255, 255 };
  }
}
const char *ImGuiToast::get_icon()
{
  switch (type) {
  case ImGuiToastType::COUNT:
  case ImGuiToastType::None:
    return NULL;
  case ImGuiToastType::Success:
    return ICON_FA_CIRCLE_CHECK;
  case ImGuiToastType::Warning:
    return ICON_FA_TRIANGLE_EXCLAMATION;
  case ImGuiToastType::Error:
    return ICON_FA_CIRCLE_XMARK;
  case ImGuiToastType::Info:
    return ICON_FA_CIRCLE_INFO;
  }
}
char *ImGuiToast::get_content() { return content; };
int64_t ImGuiToast::get_elapsed_time()
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
           .count()
         - creation_time;
}

ImGuiToastPhase ImGuiToast::get_phase()
{
  const auto elapsed = get_elapsed_time();

  if (elapsed > NOTIFY_FADE_IN_OUT_TIME + dismiss_time + NOTIFY_FADE_IN_OUT_TIME) {
    return ImGuiToastPhase::Expired;
  } else if (elapsed > NOTIFY_FADE_IN_OUT_TIME + dismiss_time) {
    return ImGuiToastPhase::FadeOut;
  } else if (elapsed > NOTIFY_FADE_IN_OUT_TIME) {
    return ImGuiToastPhase::Wait;
  } else {
    return ImGuiToastPhase::FadeIn;
  }
}

float ImGuiToast::get_fade_percent()
{
  const auto phase = get_phase();
  const float elapsed = static_cast<float>(get_elapsed_time());

  if (phase == ImGuiToastPhase::FadeIn) {
    return (elapsed / NOTIFY_FADE_IN_OUT_TIME) * NOTIFY_OPACITY;
  } else if (phase == ImGuiToastPhase::FadeOut) {
    return (1.f - ((elapsed - NOTIFY_FADE_IN_OUT_TIME - dismiss_time) / NOTIFY_FADE_IN_OUT_TIME)) * NOTIFY_OPACITY;
  }

  return 1.f * NOTIFY_OPACITY;
}

void ImGuiToast::set_title(const char *format, va_list args) { vsnprintf(title, sizeof(title), format, args); }
void ImGuiToast::set_content(const char *format, va_list args) { vsnprintf(content, sizeof(content), format, args); }
}// namespace Anterle

namespace ImGui {
std::vector<Anterle::ImGuiToast> notifications;

void InsertNotification(const Anterle::ImGuiToast &toast) { notifications.push_back(toast); }

void RemoveNotification(int index) { notifications.erase(notifications.begin() + index); }

void RenderNotifications()
{
  const auto vp_size = GetMainViewport()->Size;

  float height = 0.f;

  for (int i = 0; i < notifications.size(); i++) {
    auto *current_toast = &notifications[i];

    if (current_toast->get_phase() == Anterle::ImGuiToastPhase::Expired) {
      RemoveNotification(i);
      continue;
    }

    const auto icon = current_toast->get_icon();
    const auto title = current_toast->get_title();
    const auto content = current_toast->get_content();
    const auto default_title = current_toast->get_default_title();
    const auto opacity = current_toast->get_fade_percent();

    auto text_color = current_toast->get_color();
    text_color.w = opacity;

    char window_name[50];
    sprintf(window_name, "##TOAST%d", i);

    SetNextWindowBgAlpha(opacity);
    SetNextWindowPos(ImVec2(vp_size.x - NOTIFY_PADDING_X, vp_size.y - NOTIFY_PADDING_Y - height),
      ImGuiCond_Always,
      ImVec2(1.0f, 1.0f));
    Begin(window_name, NULL, NOTIFY_TOAST_FLAGS);

    {
      PushTextWrapPos(vp_size.x / 3.f);

      bool was_title_rendered = false;

      if (!NOTIFY_NULL_OR_EMPTY(icon)) {
        TextColored(text_color, "%s", icon);
        was_title_rendered = true;
      }

      if (!NOTIFY_NULL_OR_EMPTY(title)) {
        if (!NOTIFY_NULL_OR_EMPTY(icon)) SameLine();

        Text("%s", title);
        was_title_rendered = true;
      } else if (!NOTIFY_NULL_OR_EMPTY(default_title)) {
        if (!NOTIFY_NULL_OR_EMPTY(icon)) SameLine();

        Text("%s", default_title);
        was_title_rendered = true;
      }

      if (was_title_rendered && !NOTIFY_NULL_OR_EMPTY(content)) { SetCursorPosY(GetCursorPosY() + 5.f); }

      if (!NOTIFY_NULL_OR_EMPTY(content)) {
        if (was_title_rendered) { Separator(); }
        Text("%s", content);
      }
      PopTextWrapPos();
    }
    height += GetWindowHeight() + NOTIFY_PADDING_MESSAGE_Y;

    End();
  }
}

void MergeIconsWithLatestFont(float font_size, bool FontDataOwnedByAtlas)
{
  static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

  ImFontConfig icons_config;
  icons_config.MergeMode = true;
  icons_config.PixelSnapH = true;
  icons_config.GlyphMinAdvanceX = 13.0f;
  icons_config.FontDataOwnedByAtlas = FontDataOwnedByAtlas;

  GetIO().Fonts->AddFontFromFileTTF("resources/fonts/fa-solid-900.ttf", font_size, &icons_config, icons_ranges);
}
}// namespace ImGui
