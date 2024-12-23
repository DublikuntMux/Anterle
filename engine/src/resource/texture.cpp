#include <glad/gl.h>

#include "resource/texture.hpp"

namespace Anterle {
Texture2D::Texture2D()
  : Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT),
    Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
  glGenTextures(1, &ID);
}

void Texture2D::Generate(uint32_t width, uint32_t height, void *data)
{
  Width = width;
  Height = height;

  glBindTexture(GL_TEXTURE_2D, ID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexImage2D(GL_TEXTURE_2D,
    0,
    Internal_Format,
    static_cast<GLsizei>(width),
    static_cast<GLsizei>(height),
    0,
    Image_Format,
    GL_UNSIGNED_BYTE,
    data);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const { glBindTexture(GL_TEXTURE_2D, ID); }
}// namespace Anterle
