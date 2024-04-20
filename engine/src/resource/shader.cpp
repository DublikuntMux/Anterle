#include <array>
#include <cstdint>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include "logger.hpp"
#include "resource/shader.hpp"

namespace Anterle {
Shader::Shader() : ID(glCreateProgram()) {}

Shader &Shader::Use()
{
  glUseProgram(ID);
  return *this;
}

void Shader::Compile(const char *vertexCode, const char *fragmentPath)
{
  unsigned int vertex = 0, fragment = 0;

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertexCode, nullptr);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragmentPath, nullptr);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");

  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);

  glLinkProgram(ID);
  checkCompileErrors(ID, "PROGRAM");

  glDeleteShader(vertex);
  glDeleteShader(fragment);
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
void Shader::SetIntegerVector(const char *name, std::vector<int> &value, int count, bool useShader)
{
  if (useShader) Use();
  glUniform1iv(glGetUniformLocation(ID, name), count, &value[0]);
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
void Shader::SetMatrix4(const char *name, const glm::mat4 &matrix, int count, bool useShader)
{
  if (useShader) Use();
  glUniformMatrix4fv(glGetUniformLocation(ID, name), count, false, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(uint32_t object, std::string type)
{
  int success = 0;
  std::array<char, 1024> infoLog{};
  if (type != "PROGRAM") {
    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(object, 1024, nullptr, infoLog.data());
      Logger::getInstance()->log(LogLevel::ERROR, "Shader compile-time error: Type: %s\n%s", type.c_str(), infoLog);
    }
  } else {
    glGetProgramiv(object, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(object, 1024, nullptr, infoLog.data());
      Logger::getInstance()->log(LogLevel::ERROR, "Shader link-time error: Type: %s\n%s", type.c_str(), infoLog);
    }
  }
}
}// namespace Anterle
