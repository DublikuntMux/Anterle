#pragma once

#include <map>
#include <string>

#include "resource/shader.hpp"
#include "resource/texture.hpp"

namespace Anterle {
class ResourceManager
{
public:
  static Shader LoadShader(std::string name);
  static Shader GetShader(std::string name);
  static Texture2D LoadTexture(std::string name, bool alpha);
  static Texture2D GetTexture(std::string name);

  static void Clear();

public:
  static std::map<std::string, Shader> Shaders;
  static std::map<std::string, Texture2D> Textures;

private:
  ResourceManager() {}

  static Shader loadShaderFromFile(std::string shaderName);
  static Texture2D loadTextureFromFile(std::string file, bool alpha);
};
}// namespace Anterle
