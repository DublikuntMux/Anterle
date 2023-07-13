#pragma once

#include <glm/glm.hpp>

#include "render/sprite_renderer.hpp"
#include "resource/texture.hpp"

class GameObject
{
public:
  glm::vec2 Position, Size, Velocity;
  glm::vec3 Color;
  float Rotation;
  bool IsSolid;
  bool Destroyed;
  Texture2D Sprite;

  GameObject();
  GameObject(glm::vec2 pos,
    glm::vec2 size,
    Texture2D sprite,
    glm::vec3 color = glm::vec3(1.0F),
    glm::vec2 velocity = glm::vec2(0.0F, 0.0F));

  virtual void Draw(SpriteRenderer &renderer);
};