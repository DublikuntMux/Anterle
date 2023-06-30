#include <algorithm>
#include <iostream>
#include <sstream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

#include "game.hpp"
#include "game_level.hpp"
#include "game_object.hpp"
#include "particle_generator.hpp"
#include "resource_manager.hpp"
#include "sprite_renderer.hpp"
#include "text_renderer.hpp"

SpriteRenderer *Renderer;
ParticleGenerator *Particles;
TextRenderer *Text;

Game::Game(uint16_t width, uint16_t height)
    : State(GAME_MENU), Configs(GameConfigs()), Width(width), Height(height),
      Keys(), KeysProcessed() {}

Game::~Game() {
  delete Renderer;
  delete Particles;
  delete Text;
}

void Game::Init() {
  ResourceManager::LoadShader("sprite");
  ResourceManager::LoadShader("particle");

  glm::mat4 projection =
      glm::ortho(0.0f, static_cast<float>(this->Width),
                 static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

  ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
  ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

  ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
  ResourceManager::GetShader("particle").SetMatrix4("projection", projection);

  ResourceManager::LoadTexture("background", false);
  ResourceManager::LoadTexture("face", true);
  ResourceManager::LoadTexture("particle", true);

  Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
  Particles =
      new ParticleGenerator(ResourceManager::GetShader("particle"),
                            ResourceManager::GetTexture("particle"), 500);
  Text = new TextRenderer(this->Width, this->Height);
  Text->Load("KingthingsClarity", 24);

  GameLevel one;
  one.Load("resources/levels/one.lvl");
  GameLevel two;
  two.Load("resources/levels/two.lvl");
  GameLevel three;
  three.Load("resources/levels/three.lvl");
  GameLevel four;
  four.Load("resources/levels/four.lvl");
  this->Levels.push_back(one);
  this->Levels.push_back(two);
  this->Levels.push_back(three);
  this->Levels.push_back(four);
  this->Configs.CurentLevel = 0;
}

void Game::Update(float dt) {
  if (this->State == GAME_ACTIVE &&
      this->Levels[this->Configs.CurentLevel].IsCompleted()) {
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
    Text->RenderText("Press W or S to select level", 245.0f,
                     this->Height / 2.0f + 20.0f, 0.75f);
  }
  if (this->State == GAME_WIN) {
    Text->RenderText("You WON!!!", 320.0f, this->Height / 2.0f - 20.0f, 1.0f,
                     glm::vec3(0.0f, 1.0f, 0.0f));
    Text->RenderText("Press ENTER to retry or ESC to quit", 130.0f,
                     this->Height / 2.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
  }
}

void Game::ResetLevel() {
  if (this->Configs.CurentLevel == 0)
    this->Levels[0].Load("resources/levels/one.lvl");
  else if (this->Configs.CurentLevel == 1)
    this->Levels[1].Load("resources/levels/two.lvl");
  else if (this->Configs.CurentLevel == 2)
    this->Levels[2].Load("resources/levels/three.lvl");
  else if (this->Configs.CurentLevel == 3)
    this->Levels[3].Load("resources/levels/four.lvl");
}
