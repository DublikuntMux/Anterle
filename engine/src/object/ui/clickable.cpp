#include <glad/gles2.h>

#include "object/ui/clickable.hpp"
#include "resource/resource_manager.hpp"

namespace Anterle {
Clickable::Clickable(uint32_t top, uint32_t left, uint32_t width, uint32_t height, ClickCallback onClick)
  : m_width(width), m_height(height), m_top(top), m_left(left), m_onClick(onClick)
{
  m_shader = ResourceManager::GetShader("clickable");

  float vertices[] = { -1.0f + 2.0f * m_left / m_width,
    1.0f - 2.0f * m_top / m_height,
    -1.0f + 2.0f * m_left / m_width,
    1.0f - 2.0f * (m_top + m_height) / m_height,
    -1.0f + 2.0f * (m_left + m_width) / m_width,
    1.0f - 2.0f * (m_top + m_height) / m_height,

    -1.0f + 2.0f * m_left / m_width,
    1.0f - 2.0f * m_top / m_height,
    -1.0f + 2.0f * (m_left + m_width) / m_width,
    1.0f - 2.0f * m_top / m_height,
    -1.0f + 2.0f * (m_left + m_width) / m_width,
    1.0f - 2.0f * (m_top + m_height) / m_height };

  uint32_t VBO;
  glGenVertexArrays(1, &m_quadVAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(m_quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
}
Clickable::~Clickable(){}

void Clickable::CheckClick(double x, double y)
{
  if (x >= m_left && x <= m_left + m_width && y >= m_top && y <= m_top + m_height) { m_onClick(); }
}

void Clickable::Render()
{
  glBindVertexArray(m_quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 4);
  glBindVertexArray(0);
}
}// namespace Anterle
