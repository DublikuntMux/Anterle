#pragma once

#include <cstdint>

#include <glm/glm.hpp>

#include "resource/shader.hpp"
#include "resource/texture.hpp"

namespace Anterle {
class SpriteRenderer
{
protected:
  SpriteRenderer(Shader shader);
  ~SpriteRenderer();

public:
  SpriteRenderer(SpriteRenderer &other) = delete;

  void operator=(const SpriteRenderer &) = delete;

  static SpriteRenderer *GetInstance();
  static SpriteRenderer *CreateInstance(Shader shader);

  void DrawSprite(Texture2D texture,
    glm::vec2 position,
    glm::vec2 size = glm::vec2(10.0f, 10.0f),
    float rotate = 0.0f,
    glm::vec3 color = glm::vec3(1.0f));

protected:
  Shader p_shader;
  uint32_t p_quadVAO;

private:
  static SpriteRenderer *_pinstance;
};
}// namespace Anterle
