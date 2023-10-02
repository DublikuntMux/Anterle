#pragma once

#include <cstdint>

#include <glm/glm.hpp>

#include "resource/shader.hpp"
#include "resource/texture.hpp"

namespace Anterle {
class SpriteRenderer
{
public:
  SpriteRenderer(Shader shader);
  ~SpriteRenderer();

  void DrawSprite(Texture2D texture,
    glm::vec2 position,
    glm::vec2 size = glm::vec2(10.0f, 10.0f),
    float rotate = 0.0f,
    glm::vec3 color = glm::vec3(1.0f));

private:
  Shader m_shader;
  uint32_t m_quadVAO;
  void initRenderData();
};
}// namespace Anterle
