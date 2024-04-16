#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace Anterle {
class Shader
{
public:
  uint32_t ID;

  Shader() {}
  Shader &Use();

  void Compile(const char *vertexCode, const char *fragmentCode);

  void SetFloat(const char *name, float value, bool useShader = false);
  void SetInteger(const char *name, int value, bool useShader = false);
  void SetIntegerVector(const char *name, std::vector<int> &value, int count, bool useShader = false);
  void SetVector2f(const char *name, float x, float y, bool useShader = false);
  void SetVector2f(const char *name, const glm::vec2 &value, bool useShader = false);
  void SetVector3f(const char *name, float x, float y, float z, bool useShader = false);
  void SetVector3f(const char *name, const glm::vec3 &value, bool useShader = false);
  void SetVector4f(const char *name, float x, float y, float z, float w, bool useShader = false);
  void SetVector4f(const char *name, const glm::vec4 &value, bool useShader = false);
  void SetMatrix4(const char *name, const glm::mat4 &matrix, bool useShader = false);
  void SetMatrix4(const char *name, const glm::mat4 &matrix, int count, bool useShader = false);

private:
  void checkCompileErrors(uint32_t object, std::string type);
};
}// namespace Anterle
