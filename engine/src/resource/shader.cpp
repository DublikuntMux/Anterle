#include <cstdint>
#include <iostream>

#include <glad/gles2.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <loguru.hpp>

#include "resource/shader.hpp"

namespace Anterle {
Shader &Shader::Use()
{
  glUseProgram(this->ID);
  return *this;
}

void Shader::Compile(const char *vertexSource, const char *fragmentSource)
{
  uint32_t sVertex, sFragment;

  sVertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(sVertex, 1, &vertexSource, NULL);
  glCompileShader(sVertex);
  checkCompileErrors(sVertex, "VERTEX");

  sFragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(sFragment, 1, &fragmentSource, NULL);
  glCompileShader(sFragment);
  checkCompileErrors(sFragment, "FRAGMENT");

  this->ID = glCreateProgram();
  glAttachShader(this->ID, sVertex);
  glAttachShader(this->ID, sFragment);
  glLinkProgram(this->ID);
  checkCompileErrors(this->ID, "PROGRAM");

  glDeleteShader(sVertex);
  glDeleteShader(sFragment);
}

void Shader::SetFloat(const char *name, float value, bool useShader)
{
  if (useShader) this->Use();
  glUniform1f(glGetUniformLocation(this->ID, name), value);
}
void Shader::SetInteger(const char *name, int value, bool useShader)
{
  if (useShader) this->Use();
  glUniform1i(glGetUniformLocation(this->ID, name), value);
}
void Shader::SetVector2f(const char *name, float x, float y, bool useShader)
{
  if (useShader) this->Use();
  glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}
void Shader::SetVector2f(const char *name, const glm::vec2 &value, bool useShader)
{
  if (useShader) this->Use();
  glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}
void Shader::SetVector3f(const char *name, float x, float y, float z, bool useShader)
{
  if (useShader) this->Use();
  glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}
void Shader::SetVector3f(const char *name, const glm::vec3 &value, bool useShader)
{
  if (useShader) this->Use();
  glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const char *name, float x, float y, float z, float w, bool useShader)
{
  if (useShader) this->Use();
  glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}
void Shader::SetVector4f(const char *name, const glm::vec4 &value, bool useShader)
{
  if (useShader) this->Use();
  glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const char *name, const glm::mat4 &matrix, bool useShader)
{
  if (useShader) this->Use();
  glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(uint32_t object, std::string type)
{
  int success;
  char infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(object, 1024, NULL, infoLog);
      LOG_F(ERROR, "Shader compile-time error: Type: %s\n%s", type.c_str(), infoLog);
    }
  } else {
    glGetProgramiv(object, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(object, 1024, NULL, infoLog);
      LOG_F(ERROR, "Shader link-time error: Type: %s\n%s", type.c_str(), infoLog);
    }
  }
}
}// namespace Anterle
