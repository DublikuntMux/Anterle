#include <cstdint>
#include <stdint.h>
#include <stdlib.h>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "logger.hpp"
#include "render/text_renderer.hpp"
#include "resource/resource_manager.hpp"
#include "resource/shader.hpp"

const unsigned int ARRAY_LIMIT = 255;

// clang-format off
GLfloat vertex_data[] = {
  0.0f,1.0f,
  0.0f,0.0f,
  1.0f,1.0f,
  1.0f,0.0f,
};
// clang-format on

namespace Anterle {
TextRenderer::TextRenderer(uint32_t width, uint32_t height)
{
  TextShader = ResourceManager::LoadShader("text");
  glm::mat4 projection = glm::ortho(0.0F, static_cast<float>(width), 0.0F, static_cast<float>(height));
  TextShader.SetMatrix4("projection", projection, true);

  glGenVertexArrays(1, &_VAO);
  glGenBuffers(1, &_VBO);
  glBindVertexArray(_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void TextRenderer::Load(std::string font)
{
  Characters.clear();

  std::string file_font = "resources/fonts/" + font + ".ttf";

  FT_Library ft = nullptr;
  if (FT_Init_FreeType(&ft) != 0) {
    Logger::getInstance()->log("Could not init FreeType Library.");
    abort();
  }

  FT_Face face = nullptr;
  if (FT_New_Face(ft, file_font.c_str(), 0, &face) != 0) {
    Logger::getInstance()->log("Failed to load font: %s", file_font.c_str());
    abort();
  }

  FT_Select_Charmap(face, ft_encoding_unicode);
  FT_Set_Pixel_Sizes(face, 256, 256);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glGenTextures(1, &textureArray);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, 256, 256, 128, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

  FT_UInt index;
  wchar_t char_code = static_cast<wchar_t>(FT_Get_First_Char(face, &index));
  while (true) {
    if (FT_Load_Char(face, char_code, FT_LOAD_RENDER) != 0) {
      Logger::getInstance()->log("Failed to load Glyph.");
      continue;
    }

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
      0,
      0,
      0,
      int(char_code),
      face->glyph->bitmap.width,
      face->glyph->bitmap.rows,
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
    Characters.insert(std::pair<char, Character>(char_code, character));

    char_code = static_cast<wchar_t>(FT_Get_Next_Char(face, char_code, &index));
    if (!index) break;
  }
  glBindTexture(GL_TEXTURE_2D, 0);

  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  for (int i = 0; i < ARRAY_LIMIT; i++) {
    letterMap.push_back(0);
    transforms.push_back(glm::mat4(1.0f));
  }
}

void TextRenderer::RenderText(const std::wstring &text, float x, float y, float scale, glm::vec3 color)
{
  scale = scale * 48.0f / 256.0f;
  float copyX = x;
  TextShader.Use();
  TextShader.SetVector3f("textColor", color);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
  glBindVertexArray(_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);

  int workingIndex = 0;

  std::wstring::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    Character ch = Characters[*c];

    if (*c == '\n') {
      y -= ((ch.Size.y)) * 1.3 * scale;
      x = copyX;
    } else if (*c == ' ') {
      x += (ch.Advance >> 6) * scale;
    } else {
      float xpos = x + ch.Bearing.x * scale;
      float ypos = y - (256 - ch.Bearing.y) * scale;

      transforms[workingIndex] = translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, 0))
                                 * glm::scale(glm::mat4(1.0f), glm::vec3(256 * scale, 256 * scale, 0));
      letterMap[workingIndex] = ch.TextureID;

      x += (ch.Advance >> 6) * scale;
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
    TextShader.SetMatrix4("transforms", transforms[0], length);
    TextShader.SetIntegerVector("letterMap", letterMap, length);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, length);
  }
}
}// namespace Anterle
