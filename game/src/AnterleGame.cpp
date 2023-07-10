#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

#include <imgui.h>
#include <imgui/imgui_impl_glfw.hpp>
#include <imgui/imgui_impl_opengl3.hpp>

#include "object/game_level.hpp"
#include "object/game_object.hpp"
#include "object/particle_generator.hpp"
#include "render/sprite_renderer.hpp"
#include "render/text_renderer.hpp"
#include "resource/resource_manager.hpp"
#include "game.hpp"

#include "AnterleGame.hpp"

std::unique_ptr<SpriteRenderer> Renderer;
std::unique_ptr<ParticleGenerator> Particles;
std::unique_ptr<TextRenderer> Text;

AnterleGame::AnterleGame(uint16_t width, uint16_t height) : Game(width, height), Configs(GameConfigs()) {}

void AnterleGame::Init()
{
  ResourceManager::LoadShader("sprite");
  ResourceManager::LoadShader("particle");

  glm::mat4 projection =
    glm::ortho(0.0F, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0F, -1.0F, 1.0F);

  ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
  ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

  ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
  ResourceManager::GetShader("particle").SetMatrix4("projection", projection);

  ResourceManager::LoadTexture("background", false);
  ResourceManager::LoadTexture("face", true);
  ResourceManager::LoadTexture("particle", true);

  Renderer = std::make_unique<SpriteRenderer>(ResourceManager::GetShader("sprite"));
  Particles = std::make_unique<ParticleGenerator>(
    ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
  Text = std::make_unique<TextRenderer>(this->Width, this->Height);
  Text->Load("KingthingsClarity", 24);

  GameLevel one;
  one.Load("one");
  GameLevel two;
  two.Load("one");
  GameLevel three;
  three.Load("one");
  GameLevel four;
  four.Load("one");
  this->Levels.push_back(one);
  this->Levels.push_back(two);
  this->Levels.push_back(three);
  this->Levels.push_back(four);
  this->Configs.CurentLevel = 0;
}

void AnterleGame::Update(float dt)
{
  if (this->State == GAME_ACTIVE && this->Levels[this->Configs.CurentLevel].IsCompleted()) {
    this->ResetLevel();
    this->State = GAME_WIN;
  }
}

void AnterleGame::ProcessInput(float dt)
{
  if (this->State == GAME_MENU) {
    if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER]) {
      this->State = GAME_ACTIVE;
      this->KeysProcessed[GLFW_KEY_ENTER] = true;
    }
    if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W]) {
      this->Configs.CurentLevel = (this->Configs.CurentLevel + 1) % 4;
      this->KeysProcessed[GLFW_KEY_W] = true;
    }
    if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S]) {
      if (this->Configs.CurentLevel > 0) {
        --this->Configs.CurentLevel;
      } else {
        this->Configs.CurentLevel = 3;
      }
      this->KeysProcessed[GLFW_KEY_S] = true;
    }
  }
  if (this->State == GAME_WIN) {
    if (this->Keys[GLFW_KEY_ENTER]) {
      this->KeysProcessed[GLFW_KEY_ENTER] = true;
      this->State = GAME_MENU;
    }
  }
}

void AnterleGame::Render()
{
  if (this->State == GAME_MENU) {
    Text->RenderText("Press ENTER to start", 250.0F, this->Height / 2.0F, 1.0F);
    
  } else if (this->State == GAME_ACTIVE) {
    Renderer->DrawSprite(ResourceManager::GetTexture("face"),
      glm::vec2(200.0F, 200.0F),
      glm::vec2(300.0F, 400.0F),
      45.0F,
      glm::vec3(0.0F, 1.0F, 0.0F));

  } else if (this->State == GAME_WIN) {
    Text->RenderText("You WON!!!", 320.0F, this->Height / 2.0F - 20.0F, 1.0F, glm::vec3(0.0F, 1.0F, 0.0F));
    Text->RenderText(
      "Press ENTER to retry or ESC to quit", 130.0F, this->Height / 2.0F, 1.0F, glm::vec3(1.0F, 1.0F, 0.0F));
  }
}

void AnterleGame::ResetLevel()
{
  if (this->Configs.CurentLevel == 0) {
    this->Levels[0].Load("one");
  } else if (this->Configs.CurentLevel == 1) {
    this->Levels[1].Load("one");
  } else if (this->Configs.CurentLevel == 2) {
    this->Levels[2].Load("one");
  } else if (this->Configs.CurentLevel == 3) {
    this->Levels[3].Load("one");
  }
}
