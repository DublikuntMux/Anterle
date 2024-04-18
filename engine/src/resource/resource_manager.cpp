#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <fpng.h>
#include <glad/glad.h>

#include "logger.hpp"
#include "resource/resource_manager.hpp"

namespace Anterle {
std::unique_ptr<ResourceManager> ResourceManager::_instance = nullptr;

std::unique_ptr<ResourceManager> &ResourceManager::getInstance()
{
  if (!_instance) { _instance = std::make_unique<ResourceManager>(); }
  return _instance;
}

Shader ResourceManager::GetShader(std::string name) { return Shaders[name]; }
Texture2D ResourceManager::GetTexture(std::string name) { return Textures[name]; }

ResourceManager::ResourceManager()
{
  fpng::fpng_init();

  for (const auto &entry : std::filesystem::recursive_directory_iterator("resources/")) {
    if (entry.path().extension() == ".png") {
      std::string name = entry.path().stem().string();
      Textures[name] = loadTextureFromFile(entry.path().string());
    } else if (entry.path().extension() == ".vert" || entry.path().extension() == ".frag") {
      std::filesystem::path path = entry.path();
      std::string name = path.stem().string();
      Shaders[name] = loadShaderFromFile(path.replace_extension("").string());
    }
  }
}

ResourceManager::~ResourceManager()
{
  for (auto &iter : Shaders) glDeleteProgram(iter.second.ID);
  for (auto &iter : Textures) glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(std::string shaderName)
{
  std::string vertexShaderFile(shaderName + ".vert");
  std::string fragmentShaderFile(shaderName + ".frag");

  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    vShaderFile.open(vertexShaderFile);
    fShaderFile.open(fragmentShaderFile);
    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  } catch (std::ifstream::failure &e) {
    Logger::getInstance()->log("Failed to read sheader: %s", e.what());
  }

  Shader shader;
  shader.Compile(vertexCode.c_str(), fragmentCode.c_str());
  return shader;
}

Texture2D ResourceManager::loadTextureFromFile(std::string file_patch)
{
  Texture2D texture;
  std::ifstream file_stream;

  file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    file_stream.open(file_patch);
  } catch (std::ifstream::failure &e) {
    Logger::getInstance()->log("Failed to load texture: %s.\nBecause: %s", file_patch.c_str(), e.what());
    abort();
  }

  uint32_t width = 0, height = 0, nrChannels = 0;
  std::vector<uint8_t> out;
  fpng::fpng_decode_file(file_patch.c_str(), out, width, height, nrChannels, 4);

  if (nrChannels > 3) {
    texture.Internal_Format = GL_RGBA;
    texture.Image_Format = GL_RGBA;
  }
  texture.Generate(width, height, out.data());

  return texture;
}
}// namespace Anterle
