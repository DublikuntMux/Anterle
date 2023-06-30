#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>

#include "spdlog/spdlog.h"
#include <glad/gl.h>

#include <fpng.hpp>

#include "resource_manager.hpp"

std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;

Shader ResourceManager::LoadShader(std::string name) {
  Shaders[name] = loadShaderFromFile(name);
  return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name) { return Shaders[name]; }

Texture2D ResourceManager::LoadTexture(std::string name, bool alpha) {
  Textures[name] = loadTextureFromFile(name, alpha);
  return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name) {
  return Textures[name];
}

void ResourceManager::Clear() {
  for (auto iter : Shaders)
    glDeleteProgram(iter.second.ID);

  for (auto iter : Textures)
    glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(std::string shaderName) {
  std::string vertexCode;
  std::string fragmentCode;

  std::ifstream vertexShaderFile;
  std::ifstream fragmentShaderFile;

  vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    vertexShaderFile.open("./resources/shaders/" + shaderName + ".vert");
    fragmentShaderFile.open("./resources/shaders/" + shaderName + ".frag");

    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vertexShaderFile.rdbuf();
    fShaderStream << fragmentShaderFile.rdbuf();

    vertexShaderFile.close();
    fragmentShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

  } catch (std::ifstream::failure &e) {
    spdlog::error("Failed to load shader: {0}", shaderName);
  }

  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();

  Shader shader;
  shader.Compile(vShaderCode, fShaderCode);
  return shader;
}

Texture2D ResourceManager::loadTextureFromFile(std::string file, bool alpha) {
  Texture2D texture;
  if (alpha) {
    texture.Internal_Format = GL_RGBA;
    texture.Image_Format = GL_RGBA;
  }
  uint32_t width, height, nrChannels;

  std::string file_patch = "./resources/textures/" + file + ".png";

  std::ifstream file_stream;
  file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    file_stream.open(file_patch);
  } catch (std::ifstream::failure &e) {
    spdlog::error("Failed to load level: {0}", file_patch);
  }

  std::vector<uint8_t> data;
  int res = fpng::fpng_decode_file(file_patch.c_str(), data, width, height, nrChannels, 0);
  if (res != fpng::FPNG_DECODE_SUCCESS)
	{
		spdlog::error("Failed to decode image!");
	}
  texture.Generate(width, height, data);
  return texture;
}
