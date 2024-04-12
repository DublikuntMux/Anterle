#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <loguru.hpp>

#include "render/sprite_renderer.hpp"

namespace Anterle {
SpriteRenderer *SpriteRenderer::_pinstance = nullptr;

SpriteRenderer *SpriteRenderer::GetInstance()
{
  if (_pinstance == nullptr) { LOG_F(ERROR, "You must create instance first!"); }
  return _pinstance;
}

SpriteRenderer *SpriteRenderer::CreateInstance(Shader shader)
{
  delete _pinstance;
  _pinstance = new SpriteRenderer(shader);
  return _pinstance;
}

SpriteRenderer::SpriteRenderer(Shader shader) : p_shader(shader)
{
  uint32_t VBO;
  float vertices[] = { 0.0f,
    1.0f,
    0.0f,
    1.0f,
    1.0f,
    0.0f,
    1.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,

    0.0f,
    1.0f,
    0.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    0.0f,
    1.0f,
    0.0f };

  glGenVertexArrays(1, &p_quadVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(p_quadVAO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

SpriteRenderer::~SpriteRenderer() { glDeleteVertexArrays(1, &p_quadVAO); }

void SpriteRenderer::DrawSprite(Texture2D texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
  p_shader.Use();
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(position, 0.0f));

  model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
  model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

  model = glm::scale(model, glm::vec3(size, 1.0f));

  p_shader.SetMatrix4("model", model);
  p_shader.SetVector3f("spriteColor", color);

  glActiveTexture(GL_TEXTURE0);
  texture.Bind();

  glBindVertexArray(p_quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}
}// namespace Anterle
