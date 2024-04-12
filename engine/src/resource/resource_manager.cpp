#include <cstdint>
#include <fstream>
#include <iostream>

#include <fpng.h>
#include <glad/gl.h>
#include <loguru.hpp>

#include "resource/resource_manager.hpp"

std::map<std::string, Anterle::Texture2D> Anterle::ResourceManager::Textures;
std::map<std::string, Anterle::Shader> Anterle::ResourceManager::Shaders;

bool fpng_inited;

namespace Anterle {
Shader ResourceManager::LoadShader(std::string name)
{
  Shaders[name] = loadShaderFromFile(name);
  return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name) { return Shaders[name]; }

Texture2D ResourceManager::LoadTexture(std::string name, bool alpha)
{
  Textures[name] = loadTextureFromFile(name, alpha);
  return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name) { return Textures[name]; }

void ResourceManager::Clear()
{
  for (auto &iter : Shaders) glDeleteProgram(iter.second.ID);

  for (auto &iter : Textures) glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(std::string shaderName)
{
  std::ifstream vertexShaderFile("./resources/shaders/" + shaderName + ".vert.spv", std::ios::binary);
  std::ifstream fragmentShaderFile("./resources/shaders/" + shaderName + ".frag.spv", std::ios::binary);

  std::istreambuf_iterator<char> vertexStartIt(vertexShaderFile), vertexEndIt;
  std::istreambuf_iterator<char> fragmentStartIt(fragmentShaderFile), fragmentEndIt;

  std::vector<char> vertexBuffer(vertexStartIt, vertexEndIt);
  std::vector<char> fragmentBuffer(fragmentStartIt, fragmentEndIt);

  vertexShaderFile.close();
  fragmentShaderFile.close();

  Shader shader;
  shader.Compile(vertexBuffer, fragmentBuffer);
  return shader;
}

Texture2D ResourceManager::loadTextureFromFile(std::string file, bool alpha)
{
  if (!fpng_inited) {
    fpng::fpng_init();
    fpng_inited = true;
  }

  Texture2D texture;
  if (alpha) {
    texture.Internal_Format = GL_RGBA;
    texture.Image_Format = GL_RGBA;
  }

  std::string file_patch = "./resources/textures/" + file + ".png";
  std::ifstream file_stream;
  file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    file_stream.open(file_patch);
  } catch (std::ifstream::failure &e) {
    ABORT_F("Failed to load level: %s", file_patch.c_str());
  }

  uint32_t width, height, nrChannels;
  std::vector<uint8_t> out;
  fpng::fpng_decode_file(file_patch.c_str(), out, width, height, nrChannels, 4);

  texture.Generate(width, height, out.data());

  return texture;
}
}// namespace Anterle
