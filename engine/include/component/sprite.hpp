#pragma once

#include <glm/glm.hpp>

#include "component/component.hpp"
#include "render/sprite_renderer.hpp"
#include "resource/texture.hpp"

namespace Anterle {
class SpriteComponent : public Component
{
public:
  glm::vec3 Color;
  glm::vec2 Size;
  Texture2D Sprite;

  SpriteComponent(GameObject *parent,
    Texture2D sprite,
    glm::vec2 size = glm::vec2(1.0F, 1.0F),
    glm::vec3 color = glm::vec3(1.0F));

  void Update();

private:
  SpriteRenderer *_renderer;
};
}// namespace Anterle
