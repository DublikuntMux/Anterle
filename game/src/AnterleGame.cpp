#include <string>

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

AnterleGame::~AnterleGame()
{
  delete Text;
  delete Audio;
}

void AnterleGame::Init()
{
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImFontConfig font_cfg;
  font_cfg.FontDataOwnedByAtlas = false;
  io.Fonts->AddFontFromFileTTF("resources/fonts/tahoma.ttf", 17.f, &font_cfg);

  ImGui::MergeIconsWithLatestFont(16.f, false);

  Anterle::ResourceManager::LoadShader("sprite");

  glm::mat4 projection = glm::ortho(0.0F, static_cast<float>(Width), static_cast<float>(Height), 0.0F, -1.0F, 1.0F);

  Anterle::ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
  Anterle::ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

  Anterle::ResourceManager::LoadTexture("background", false);
  Anterle::ResourceManager::LoadTexture("face", true);

  Renderer = Anterle::SpriteRenderer::CreateInstance(Anterle::ResourceManager::GetShader("sprite"));
  Text = new Anterle::TextRenderer(Width, Height);
  Text->Load("tahoma", 24);
  Audio = new Anterle::AudioSystem();

  Anterle::GameLevel one("one");
  Anterle::GameLevel two("one");
  Anterle::GameLevel three("one");
  Anterle::GameLevel four("one");
  Levels.push_back(one);
  Levels.push_back(two);
  Levels.push_back(three);
  Levels.push_back(four);
  Configs->CurentLevel = 0;
}

void AnterleGame::Update()
{
  if (State == Anterle::GameState::GAME_ACTIVE && Levels[Configs->CurentLevel].IsCompleted()) {
    ResetLevel();
    State = Anterle::GameState::GAME_WIN;
  }
}

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
    Renderer->DrawSprite(Anterle::ResourceManager::GetTexture("face"),
      glm::vec2(200.0F, 200.0F),
      glm::vec2(300.0F, 400.0F),
      45.0F,
      glm::vec3(0.0F, 1.0F, 0.0F));

  } else if (State == Anterle::GameState::GAME_WIN) {
    Text->RenderText(L"You WON!!!", 320.0F, Height / 2.0F - 20.0F, 1.0F, glm::vec3(0.0F, 1.0F, 0.0F));
    Text->RenderText(L"Press ENTER to retry or ESC to quit", 130.0F, Height / 2.0F, 1.0F, glm::vec3(1.0F, 1.0F, 0.0F));
  }
}

void AnterleGame::ResetLevel()
{
  if (Configs->CurentLevel == 0) {
    Levels[0].Reset();
  } else if (Configs->CurentLevel == 1) {
    Levels[1].Reset();
  } else if (Configs->CurentLevel == 2) {
    Levels[2].Reset();
  } else if (Configs->CurentLevel == 3) {
    Levels[3].Reset();
  }
}
