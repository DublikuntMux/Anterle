#pragma once

#include <map>
#include <memory>
#include <string>

#include "resource/shader.hpp"
#include "resource/texture.hpp"

namespace Anterle {
class ResourceManager
{
public:
  ResourceManager() = default;
  ~ResourceManager();

  ResourceManager(const ResourceManager &) = delete;
  ResourceManager &operator=(const ResourceManager &) = delete;
  ResourceManager(ResourceManager &&) = delete;
  ResourceManager &operator=(ResourceManager &&) = delete;

  static std::unique_ptr<ResourceManager> &getInstance();

  Shader LoadShader(std::string name);
  Shader GetShader(std::string name);
  Texture2D LoadTexture(std::string name, bool alpha);
  Texture2D GetTexture(std::string name);

public:
  std::map<std::string, Shader> Shaders;
  std::map<std::string, Texture2D> Textures;

private:
  Shader loadShaderFromFile(std::string shaderName);
  Texture2D loadTextureFromFile(std::string file, bool alpha);

private:
  static std::unique_ptr<ResourceManager> _instance;
  bool fpng_inited;
};
}// namespace Anterle
