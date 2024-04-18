#include <array>

#include <glad/glad.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "render/sprite_renderer.hpp"

namespace Anterle {

// clang-format off
const std::array<float, 24> vertices = {
0.0f,1.0f, 0.0f, 1.0f, 
1.0f,0.0f, 1.0f, 0.0f,
0.0f,0.0f, 0.0f,0.0f,
0.0f,1.0f, 0.0f, 1.0f,
1.0f,1.0f, 1.0f, 1.0f,
1.0f,0.0f, 1.0f, 0.0f
};
// clang-format on

SpriteRenderer::SpriteRenderer(Shader shader) : _shader(shader)
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

  glBindVertexArray(VAO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

SpriteRenderer::~SpriteRenderer()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

void SpriteRenderer::DrawSprite(Texture2D texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
  _shader.Use();
  auto model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(position, 0.0f));

  model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
  model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

  model = glm::scale(model, glm::vec3(size, 1.0f));

  _shader.SetMatrix4("model", model);
  _shader.SetVector3f("spriteColor", color);

  glActiveTexture(GL_TEXTURE0);
  texture.Bind();

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}
}// namespace Anterle
