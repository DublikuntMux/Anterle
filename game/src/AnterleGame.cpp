#include <memory>

#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <imgui.h>

#include <anterle_engine.hpp>

#include "AnterleGame.hpp"

AnterleGame::AnterleGame(uint16_t width, uint16_t height, const char *title)
  : Game(width, height, title), Configs(new GameConfigs())
{}
AnterleGame::AnterleGame(uint16_t width, uint16_t height, const char *title, GameConfigs *Configs)
  : Game(width, height, title), Configs(Configs)
{}

void AnterleGame::Init()
{
  Game::Init();

  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImFontConfig font_cfg;
  font_cfg.FontDataOwnedByAtlas = false;
  io.Fonts->AddFontFromFileTTF("resources/fonts/tahoma.ttf", 17.f, &font_cfg);

  ImGui::MergeIconsWithLatestFont(16.f, false);

  auto &resource = Anterle::ResourceManager::getInstance();
  resource->LoadTexture("background", false);
  resource->LoadTexture("face", true);

  Configs->CurentLevel = 0;
}

void AnterleGame::Update() {}

void AnterleGame::FixedUpdate() {}

void AnterleGame::ProcessInput()
{
  if (State == Anterle::GameState::GAME_MENU) {
    if (Keys[GLFW_KEY_ENTER] && !KeysProcessed[GLFW_KEY_ENTER]) {
      State = Anterle::GameState::GAME_ACTIVE;
      KeysProcessed[GLFW_KEY_ENTER] = true;
    }
  }
  if (State == Anterle::GameState::GAME_WIN) {
    if (Keys[GLFW_KEY_ENTER]) {
      KeysProcessed[GLFW_KEY_ENTER] = true;
      State = Anterle::GameState::GAME_MENU;
    }
  }
  if (Keys[GLFW_KEY_F11] && !KeysProcessed[GLFW_KEY_F11]) {
    saveScreenshot("screenshot.png");
    ImGui::InsertNotification({ Anterle::ImGuiToastType::Info, 3000, "Screenshot created!" });

    KeysProcessed[GLFW_KEY_F11] = true;
  }
}

void AnterleGame::Render()
{
  if (State == Anterle::GameState::GAME_MENU) {
    Text->RenderText(L"Press ENTER to start", 250.0F, Height / 2.0F, 1.0F);

  } else if (State == Anterle::GameState::GAME_ACTIVE) {
    Renderer->DrawSprite(Anterle::ResourceManager::getInstance()->GetTexture("face"),
      glm::vec2(200.0F, 200.0F),
      glm::vec2(300.0F, 400.0F),
      45.0F,
      glm::vec3(0.0F, 1.0F, 0.0F));

  } else if (State == Anterle::GameState::GAME_WIN) {
    Text->RenderText(L"You WON!!!", 320.0F, Height / 2.0F - 20.0F, 1.0F, glm::vec3(0.0F, 1.0F, 0.0F));
    Text->RenderText(L"Press ENTER to retry or ESC to quit", 130.0F, Height / 2.0F, 1.0F, glm::vec3(1.0F, 1.0F, 0.0F));
  }
}
