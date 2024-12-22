#include <array>
#include <cstdint>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <SDL.h>
#include <glad/gl.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "logger.hpp"
#include "render/text_renderer.hpp"
#include "resource/resource_manager.hpp"
#include "resource/shader.hpp"
#include "utils.hpp"

const int ARRAY_LIMIT = 255;

// clang-format off
const std::array<GLfloat, 8> vertex_data = {
  0.0f,1.0f,
  0.0f,0.0f,
  1.0f,1.0f,
  1.0f,0.0f,
};
// clang-format on

namespace Anterle {
TextRenderer::TextRenderer(uint32_t width, uint32_t height)
{
  _textShader = ResourceManager::getInstance()->GetShader("text");
  glm::mat4 projection = glm::ortho(0.0F, static_cast<float>(width), 0.0F, static_cast<float>(height));
  _textShader.SetMatrix4("projection", projection, true);

  glGenVertexArrays(1, &_VAO);
  glGenBuffers(1, &_VBO);
  glBindVertexArray(_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void TextRenderer::Load(std::string font)
{
  _characters.clear();

  std::string file_font = "fonts/" + font + ".ttf";

  FT_Library ft = nullptr;
  if (FT_Init_FreeType(&ft) != 0) {
    Logger::getInstance()->log(LogLevel::CRITICAL, "Could not init FreeType Library.");
  }

  auto [fontBuffer, fileSize] = Utils::LoadRawDataFromFile(file_font);
  if (fontBuffer == nullptr) {
    Logger::getInstance()->log(LogLevel::CRITICAL, "Failed to load font file: %s", file_font.c_str());
  }

  FT_Face face = nullptr;
  if (FT_New_Memory_Face(ft, reinterpret_cast<FT_Byte *>(fontBuffer.get()), static_cast<FT_Long>(fileSize), 0, &face)
      != 0) {
    Logger::getInstance()->log(LogLevel::CRITICAL, "Failed to load font from memory.");
  }

  FT_Select_Charmap(face, ft_encoding_unicode);
  FT_Set_Pixel_Sizes(face, 256, 256);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glGenTextures(1, &_textureArray);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, _textureArray);
  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, 256, 256, 128, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

  FT_UInt index = 0;
  auto char_code = static_cast<wchar_t>(FT_Get_First_Char(face, &index));
  while (true) {
    if (FT_Load_Char(face, char_code, FT_LOAD_RENDER | FT_LOAD_TARGET_LIGHT) != 0) {
      Logger::getInstance()->log(LogLevel::ERROR, "Failed to load Glyph.");
      continue;
    }

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
      0,
      0,
      0,
      int(char_code),
      static_cast<GLsizei>(face->glyph->bitmap.width),
      static_cast<GLsizei>(face->glyph->bitmap.rows),
      1,
      GL_RED,
      GL_UNSIGNED_BYTE,
      face->glyph->bitmap.buffer);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Character character = { char_code,
      glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
      glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
      static_cast<unsigned int>(face->glyph->advance.x) };
    _characters.insert(std::pair<char, Character>(char_code, character));

    char_code = static_cast<wchar_t>(FT_Get_Next_Char(face, char_code, &index));
    if (!index) break;
  }
  glBindTexture(GL_TEXTURE_2D, 0);

  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  for (int i = 0; i < ARRAY_LIMIT; i++) {
    _letterMap.push_back(0);
    _transforms.emplace_back(1.0f);
  }
}

void TextRenderer::RenderText(const std::wstring &text, float x, float y, float scale, glm::vec3 color)
{
  scale = scale * 48.0f / 256.0f;
  float copyX = x;
  _textShader.Use();
  _textShader.SetVector3f("textColor", color);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, _textureArray);
  glBindVertexArray(_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);

  int workingIndex = 0;

  std::wstring::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    Character ch = _characters[*c];

    if (*c == '\n') {
      y -= static_cast<float>(ch.Size.y) * 1.3f * scale;
      x = copyX;
    } else if (*c == ' ') {
      x += static_cast<float>(ch.Advance >> 6) * scale;
    } else {
      float xpos = x + static_cast<float>(ch.Bearing.x) * scale;
      float ypos = y - static_cast<float>(256 - ch.Bearing.y) * scale;

      _transforms[workingIndex] = translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, 0))
                                  * glm::scale(glm::mat4(1.0f), glm::vec3(256 * scale, 256 * scale, 0));
      _letterMap[workingIndex] = static_cast<int>(ch.TextureID);

      x += static_cast<float>(ch.Advance >> 6) * scale;
      workingIndex++;
      if (workingIndex == ARRAY_LIMIT) {
        TextRenderCall(workingIndex);
        workingIndex = 0;
      }
    }
  }
  TextRenderCall(workingIndex);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void TextRenderer::TextRenderCall(int length)
{
  if (length != 0) {
    _textShader.SetMatrix4("transforms", _transforms[0], length);
    _textShader.SetIntegerVector("letterMap", _letterMap, length);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, length);
  }
}
}// namespace Anterle
