#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>

#include <glad/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

#include "resource/resource_manager.hpp"

std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;

Shader ResourceManager::LoadShader(std::string name, bool useGeometry)
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

Shader ResourceManager::loadShaderFromFile(std::string shaderName, bool useGeometry)
{
  std::string vertexCode;
  std::string fragmentCode;
  std::string geometryCode;

  std::ifstream vertexShaderFile;
  std::ifstream fragmentShaderFile;
  std::ifstream geometryShaderFile;

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

    if (useGeometry) {
      geometryShaderFile.open("./resources/shaders/" + shaderName + ".geom");

      std::stringstream gShaderStream;

      gShaderStream << geometryShaderFile.rdbuf();
      geometryShaderFile.close();

      geometryCode = gShaderStream.str();
    }

  } catch (std::ifstream::failure &e) {
    printf("[Anterle Engine] Failed to load shader: %s\n", shaderName.c_str());
  }

  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();
  const char *gShaderCode = geometryCode.c_str();

  Shader shader;
  shader.Compile(vShaderCode, fShaderCode, useGeometry != false ? gShaderCode : nullptr);
  return shader;
}

Texture2D ResourceManager::loadTextureFromFile(std::string file, bool alpha)
{
  Texture2D texture;
  if (alpha) {
    texture.Internal_Format = GL_RGBA;
    texture.Image_Format = GL_RGBA;
  }
  int width, height, nrChannels;

  std::string file_patch = "./resources/textures/" + file + ".png";

  std::ifstream file_stream;
  file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    file_stream.open(file_patch);
  } catch (std::ifstream::failure &e) {
    printf("[Anterle Engine] Failed to load level: %s\n", file_patch.c_str());
  }

  uint8_t *data = stbi_load(file_patch.c_str(), &width, &height, &nrChannels, 0);
  texture.Generate(width, height, data);
  return texture;
}
