#include <cstdint>
#include <stdint.h>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/gles2.h>
#include <glm/gtc/matrix_transform.hpp>
#include <loguru.hpp>

#include "render/text_renderer.hpp"
#include "resource/resource_manager.hpp"

namespace Anterle {
TextRenderer::TextRenderer(uint32_t width, uint32_t height)
{
  TextShader = ResourceManager::LoadShader("text");
  TextShader.SetMatrix4(
    "projection", glm::ortho(0.0F, static_cast<float>(width), static_cast<float>(height), 0.0F), true);
  TextShader.SetInteger("text", 0);

  glGenVertexArrays(1, &_VAO);
  glGenBuffers(1, &_VBO);
  glBindVertexArray(_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void TextRenderer::Load(std::string font, uint32_t fontSize)
{
  Characters.clear();

  std::string file_font = "resources/fonts/" + font + ".ttf";

  FT_Library ft = nullptr;
  if (FT_Init_FreeType(&ft) != 0) { ABORT_F("Could not init FreeType Library."); }

  FT_Face face = nullptr;
  if (FT_New_Face(ft, file_font.c_str(), 0, &face) != 0) { ABORT_F("Failed to load font: %s", file_font.c_str()); }

  FT_Select_Charmap(face, ft_encoding_unicode);
  FT_Set_Pixel_Sizes(face, 0, fontSize);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  FT_UInt index;
  wchar_t character = static_cast<wchar_t>(FT_Get_First_Char(face, &index));
  while (true) {
    if (FT_Load_Char(face, character, FT_LOAD_RENDER) != 0) {
      LOG_F(ERROR, "Failed to load Glyph.");
      continue;
    }

    uint32_t texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D,
      0,
      GL_RED,
      static_cast<GLsizei>(face->glyph->bitmap.width),
      static_cast<GLsizei>(face->glyph->bitmap.rows),
      0,
      GL_RED,
      GL_UNSIGNED_BYTE,
      face->glyph->bitmap.buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Character character_form = { texture,
      glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
      glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
      face->glyph->advance.x };
    Characters.insert(std::pair<wchar_t, Character>(character, character_form));

    character = static_cast<wchar_t>(FT_Get_Next_Char(face, character, &index));
    if (!index) break;
  }
  glBindTexture(GL_TEXTURE_2D, 0);

  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(const std::wstring &text, float x, float y, float scale, glm::vec3 color)
{
  TextShader.Use();
  TextShader.SetVector3f("textColor", color);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(_VAO);

  std::wstring::const_iterator character_iter;
  for (character_iter = text.begin(); character_iter != text.end(); character_iter++) {
    Character character = Characters[*character_iter];

    float xpos = x + static_cast<float>(character.Bearing.x) * scale;
    float ypos = y + static_cast<float>(Characters['H'].Bearing.y - character.Bearing.y) * scale;

    float w = static_cast<float>(character.Size.x) * scale;
    float h = static_cast<float>(character.Size.y) * scale;

    float vertices[6][4] = { { xpos, ypos + h, 0.0F, 1.0F },
      { xpos + w, ypos, 1.0F, 0.0F },
      { xpos, ypos, 0.0F, 0.0F },

      { xpos, ypos + h, 0.0F, 1.0F },
      { xpos + w, ypos + h, 1.0F, 1.0F },
      { xpos + w, ypos, 1.0F, 0.0F } };

    glBindTexture(GL_TEXTURE_2D, character.TextureID);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    x += static_cast<float>(character.Advance >> 6) * scale;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
}// namespace Anterle
