#pragma once

#include <cstdint>
#include <unordered_map>

#include <glm/glm.hpp>

#include "resource/shader.hpp"
#include "resource/texture.hpp"

namespace Anterle {
struct SpriteInstance
{
  Texture2D texture;
  glm::vec2 position;
  glm::vec2 size;
  float rotation;
  glm::vec3 color;

  SpriteInstance(const Texture2D &texture,
    const glm::vec2 &position,
    const glm::vec2 &size = glm::vec2(10.0f, 10.0f),
    float rotation = 0.0f,
    const glm::vec3 &color = glm::vec3(1.0f))
    : texture(texture), position(position), size(size), rotation(rotation), color(color)
  {}
};

class SpriteRenderer
{
public:
  SpriteRenderer(Shader shader);
  ~SpriteRenderer();

  SpriteRenderer(const SpriteRenderer &) = default;
  SpriteRenderer &operator=(const SpriteRenderer &) = default;
  SpriteRenderer(SpriteRenderer &&) noexcept = default;
  SpriteRenderer &operator=(SpriteRenderer &&) noexcept = default;

  void AddInstance(const SpriteInstance &instance);
  void RenderBatches();

private:
  Shader _shader;
  uint32_t VBO = 0, VAO = 0;
  std::unordered_map<uint32_t, std::vector<SpriteInstance>> instanceBatches;
};
}// namespace Anterle
