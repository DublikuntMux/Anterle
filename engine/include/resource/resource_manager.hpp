#pragma once

#include <map>
#include <memory>
#include <string>

#include "audio.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"

namespace Anterle {
class ResourceManager
{
public:
  ResourceManager();
  ~ResourceManager();

  ResourceManager(const ResourceManager &) = delete;
  ResourceManager &operator=(const ResourceManager &) = delete;
  ResourceManager(ResourceManager &&) = delete;
  ResourceManager &operator=(ResourceManager &&) = delete;

  static std::unique_ptr<ResourceManager> &getInstance();

  Shader GetShader(std::string name);
  Texture2D GetTexture(std::string name);
  Audio GetAudio(std::string name);

private:
  Shader loadShaderFromFile(std::string shaderName);
  Texture2D loadTextureFromFile(std::string filePath);
  Audio loadAudioFromFile(std::string filePath);

private:
  std::map<std::string, Shader> Shaders;
  std::map<std::string, Texture2D> Textures;
  std::map<std::string, Audio> Audios;

  static std::unique_ptr<ResourceManager> _instance;
};
}// namespace Anterle
