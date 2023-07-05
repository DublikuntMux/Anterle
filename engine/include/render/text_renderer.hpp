#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "resource/shader.hpp"
#include "resource/texture.hpp"

struct Character
{
  uint32_t TextureID;
  glm::ivec2 Size;
  glm::ivec2 Bearing;
  int64_t Advance;
};

class TextRenderer
{
public:
  std::unordered_map<char, Character> Characters;
  Shader TextShader;

  TextRenderer(uint32_t width, uint32_t height);

  void Load(std::string font, uint32_t fontSize);
  void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));

private:
  uint32_t _VAO, _VBO;
};
