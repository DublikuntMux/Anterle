#include <algorithm>
#include <iostream>
#include <sstream>

#include "imgui/imgui_impl_glfw.hpp"
#include "imgui/imgui_impl_opengl3.hpp"
#include <imgui.h>

#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

#include "game.hpp"
#include "object/game_level.hpp"
#include "object/game_object.hpp"
#include "object/particle_generator.hpp"
#include "render/sprite_renderer.hpp"
#include "render/text_renderer.hpp"
#include "resource/resource_manager.hpp"

SpriteRenderer *Renderer;
ParticleGenerator *Particles;
TextRenderer *Text;

Game::Game(uint16_t width, uint16_t height)
    : State(GAME_MENU), Configs(GameConfigs()), Width(width), Height(height), Keys(), KeysProcessed() {}

Game::~Game() {
  delete Renderer;
  delete Particles;
  delete Text;
}

void Game::Init() {
  ResourceManager::LoadShader("sprite");
  ResourceManager::LoadShader("particle");

  glm::mat4 projection =
      glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

  ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
  ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

  ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
  ResourceManager::GetShader("particle").SetMatrix4("projection", projection);

  ResourceManager::LoadTexture("background", false);
  ResourceManager::LoadTexture("face", true);
  ResourceManager::LoadTexture("particle", true);

  Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
  Particles =
      new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
  Text = new TextRenderer(this->Width, this->Height);
  Text->Load("KingthingsClarity", 24);

  GameLevel one;
  one.Load("one");
  GameLevel two;
  two.Load("two");
  GameLevel three;
  three.Load("three");
  GameLevel four;
  four.Load("four");
  this->Levels.push_back(one);
  this->Levels.push_back(two);
  this->Levels.push_back(three);
  this->Levels.push_back(four);
  this->Configs.CurentLevel = 0;
}

void Game::Update(float dt) {
  if (this->State == GAME_ACTIVE && this->Levels[this->Configs.CurentLevel].IsCompleted()) {
    this->ResetLevel();
    this->State = GAME_WIN;
  }
}

void Game::ProcessInput(float dt) {
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
      if (this->Configs.CurentLevel > 0)
        --this->Configs.CurentLevel;
      else
        this->Configs.CurentLevel = 3;
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

void Game::Render() {
  if (this->State == GAME_MENU) {
    Text->RenderText("Press ENTER to start", 250.0f, this->Height / 2.0f, 1.0f);
  } else if (this->State == GAME_ACTIVE) {
    Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f),
                         45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  } else if (this->State == GAME_WIN) {
    Text->RenderText("You WON!!!", 320.0f, this->Height / 2.0f - 20.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    Text->RenderText("Press ENTER to retry or ESC to quit", 130.0f, this->Height / 2.0f, 1.0f,
                     glm::vec3(1.0f, 1.0f, 0.0f));
  }
}

void Game::ResetLevel() {
  if (this->Configs.CurentLevel == 0)
    this->Levels[0].Load("one");
  else if (this->Configs.CurentLevel == 1)
    this->Levels[1].Load("two");
  else if (this->Configs.CurentLevel == 2)
    this->Levels[2].Load("three");
  else if (this->Configs.CurentLevel == 3)
    this->Levels[3].Load("four");
}
