#include <cstdint>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/gl.h>
#include <loguru.hpp>

#include "resource/shader.hpp"

namespace Anterle {
Shader &Shader::Use()
{
  glUseProgram(ID);
  return *this;
}

void Shader::Compile(std::vector<char> &vertexSource, std::vector<char> &fragmentSource)
{
  uint32_t sVertex, sFragment;

  sVertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderBinary(1, &sVertex, GL_SHADER_BINARY_FORMAT_SPIR_V, vertexSource.data(), vertexSource.size());
  glSpecializeShader(sVertex, "main", 0, 0, 0);

  sFragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderBinary(1, &sFragment, GL_SHADER_BINARY_FORMAT_SPIR_V, fragmentSource.data(), fragmentSource.size());
  glSpecializeShader(sFragment, "main", 0, 0, 0);

  ID = glCreateProgram();
  glAttachShader(ID, sVertex);
  glAttachShader(ID, sFragment);
  glLinkProgram(ID);
  checkCompileErrors(ID, "PROGRAM");

  glDeleteShader(sVertex);
  glDeleteShader(sFragment);
}

void Shader::SetFloat(const char *name, float value, bool useShader)
{
  if (useShader) Use();
  glUniform1f(glGetUniformLocation(ID, name), value);
}
void Shader::SetInteger(const char *name, int value, bool useShader)
{
  if (useShader) Use();
  glUniform1i(glGetUniformLocation(ID, name), value);
}
void Shader::SetVector2f(const char *name, float x, float y, bool useShader)
{
  if (useShader) Use();
  glUniform2f(glGetUniformLocation(ID, name), x, y);
}
void Shader::SetVector2f(const char *name, const glm::vec2 &value, bool useShader)
{
  if (useShader) Use();
  glUniform2f(glGetUniformLocation(ID, name), value.x, value.y);
}
void Shader::SetVector3f(const char *name, float x, float y, float z, bool useShader)
{
  if (useShader) Use();
  glUniform3f(glGetUniformLocation(ID, name), x, y, z);
}
void Shader::SetVector3f(const char *name, const glm::vec3 &value, bool useShader)
{
  if (useShader) Use();
  glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const char *name, float x, float y, float z, float w, bool useShader)
{
  if (useShader) Use();
  glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
}
void Shader::SetVector4f(const char *name, const glm::vec4 &value, bool useShader)
{
  if (useShader) Use();
  glUniform4f(glGetUniformLocation(ID, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const char *name, const glm::mat4 &matrix, bool useShader)
{
  if (useShader) Use();
  glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, false, glm::value_ptr(matrix));
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
