#include "component/sprite.hpp"
#include "component/component.hpp"
#include "object/game_object.hpp"
#include "render/sprite_renderer.hpp"

namespace Anterle {
SpriteComponent::SpriteComponent(GameObject *parent, Texture2D sprite, glm::vec2 size, glm::vec3 color)
  : Component(parent), Color(color), Size(size), Sprite(sprite)
{
  _renderer = SpriteRenderer::GetInstance();
}

void SpriteComponent::Update() { _renderer->DrawSprite(Sprite, p_parent->Position, Size, p_parent->Rotation, Color); }
}// namespace Anterle
