#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "resource/shader.hpp"

namespace Anterle {
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
  TextRenderer(uint32_t width, uint32_t height);

  void Load(std::string font);
  void RenderText(const std::wstring &text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0F));

private:
  void TextRenderCall(int length);

  Shader _textShader;
  uint32_t _VAO, _VBO;

  std::unordered_map<wchar_t, Character> _characters;
  uint32_t _textureArray;
  std::vector<glm::mat4> _transforms;
  std::vector<int> _letterMap;
};
}// namespace Anterle
