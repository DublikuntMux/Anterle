#include <glad/gl.h>

#include "object/ui/clickable.hpp"
#include "resource/resource_manager.hpp"

namespace Anterle {
Clickable::Clickable(uint32_t top, uint32_t left, uint32_t width, uint32_t height, ClickCallback onClick)
  : _width(width), _height(height), _top(top), _left(left), _onClick(onClick)
{
  _shader = ResourceManager::GetShader("clickable");

  float vertices[] = { -1.0f + 2.0f * _left / _width,
    1.0f - 2.0f * _top / _height,
    -1.0f + 2.0f * _left / _width,
    1.0f - 2.0f * (_top + _height) / _height,
    -1.0f + 2.0f * (_left + _width) / _width,
    1.0f - 2.0f * (_top + _height) / _height,

    -1.0f + 2.0f * _left / _width,
    1.0f - 2.0f * _top / _height,
    -1.0f + 2.0f * (_left + _width) / _width,
    1.0f - 2.0f * _top / _height,
    -1.0f + 2.0f * (_left + _width) / _width,
    1.0f - 2.0f * (_top + _height) / _height };

  uint32_t VBO;
  glGenVertexArrays(1, &_quadVAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(_quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
}
Clickable::~Clickable() {}

void Clickable::CheckClick(double x, double y)
{
  if (x >= _left && x <= _left + _width && y >= _top && y <= _top + _height) { _onClick(); }
}

void Clickable::Render()
{
  glBindVertexArray(_quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 4);
  glBindVertexArray(0);
}
}// namespace Anterle
