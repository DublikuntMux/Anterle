#pragma once

#include <map>
#include <string>

#include "shader.hpp"
#include "texture.hpp"

class ResourceManager
{
public:
  static std::map<std::string, Shader> Shaders;
  static std::map<std::string, Texture2D> Textures;

  static Shader LoadShader(std::string name, bool useGeometry = false);
  static Shader GetShader(std::string name);
  static Texture2D LoadTexture(std::string name, bool alpha);
  static Texture2D GetTexture(std::string name);

  static void Clear();

private:
  ResourceManager() {}

  static Shader loadShaderFromFile(std::string shaderName, bool useGeometry = false);
  static Texture2D loadTextureFromFile(std::string file, bool alpha);
};
