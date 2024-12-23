#include <string>

#include <glad/gl.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "logger.hpp"
#include "resource/audio.hpp"
#include "resource/resource_manager.hpp"
#include "utils.hpp"

namespace Anterle {
std::unique_ptr<ResourceManager> ResourceManager::_instance = nullptr;

std::unique_ptr<ResourceManager> &ResourceManager::getInstance()
{
  if (!_instance) { _instance = std::make_unique<ResourceManager>(); }
  return _instance;
}

Shader ResourceManager::GetShader(std::string name)
{
  auto it = Shaders.find(name);
  if (it != Shaders.end()) {
    return it->second;
  } else {
    Shader shader = loadShaderFromFile(name);
    Shaders[name] = shader;
    return shader;
  }
}

Texture2D ResourceManager::GetTexture(std::string name)
{
  auto it = Textures.find(name);
  if (it != Textures.end()) {
    return it->second;
  } else {
    Texture2D texture = loadTextureFromFile(name);
    Textures[name] = texture;
    return texture;
  }
}

Audio ResourceManager::GetAudio(std::string name)
{
  auto it = Audios.find(name);
  if (it != Audios.end()) {
    return it->second;
  } else {
    Audio audio = loadAudioFromFile(name);
    Audios[name] = audio;
    return audio;
  }
}

ResourceManager::ResourceManager()
{
  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    Logger::getInstance()->log(LogLevel::CRITICAL, "SDL image could not initialize! Error: %s", IMG_GetError());
  }
}

ResourceManager::~ResourceManager()
{
  for (auto &iter : Shaders) glDeleteProgram(iter.second.ID);
  for (auto &iter : Textures) glDeleteTextures(1, &iter.second.ID);
  for (auto &iter : Audios) Mix_FreeChunk((Mix_Chunk *)iter.second.chunk);

  IMG_Quit();
}

Shader ResourceManager::loadShaderFromFile(std::string shaderName)
{
  std::string vertexShaderFile("shaders/" + shaderName + ".vert");
  std::string fragmentShaderFile("shaders/" + shaderName + ".frag");

  auto [vertexShaderBuffer, vertexShaderSize] = Utils::LoadRawDataFromFile(vertexShaderFile);
  if (vertexShaderBuffer == nullptr) {
    Logger::getInstance()->log(LogLevel::ERROR, "Failed to load vertex shader file: %s", vertexShaderFile.c_str());
  }

  auto [fragmentShaderBuffer, fragmentShaderSize] = Utils::LoadRawDataFromFile(fragmentShaderFile);
  if (fragmentShaderBuffer == nullptr) {
    Logger::getInstance()->log(LogLevel::ERROR, "Failed to load fragment shader file: %s", fragmentShaderFile.c_str());
  }

  Shader shader;
  shader.Compile(vertexShaderBuffer.get(), fragmentShaderBuffer.get());
  return shader;
}

Texture2D ResourceManager::loadTextureFromFile(std::string filePath)
{
  Texture2D texture;

#ifdef __ANDROID__
  std::string prefix = "textures/";
#else
  std::string prefix = "assets/textures/";
#endif

  SDL_RWops *rw = SDL_RWFromFile((prefix + filePath + ".png").c_str(), "rb");

  if (rw == nullptr) {
    Logger::getInstance()->log(LogLevel::ERROR, "Unable to load image %s! Error: %s", filePath.c_str(), SDL_GetError());
    return texture;
  }

  SDL_Surface *loadedSurface = IMG_Load_RW(rw, 1);
  if (loadedSurface == nullptr) {
    Logger::getInstance()->log(LogLevel::ERROR, "Unable to load image %s! Error: %s", filePath.c_str(), IMG_GetError());
    SDL_RWclose(rw);
    return texture;
  }

  if (loadedSurface->format->Amask != 0) {
    texture.Internal_Format = GL_RGBA;
    texture.Image_Format = GL_RGBA;
  }
  texture.Generate(loadedSurface->w, loadedSurface->h, loadedSurface->pixels);

  SDL_FreeSurface(loadedSurface);
  SDL_RWclose(rw);

  return texture;
}

Audio ResourceManager::loadAudioFromFile(std::string filePath)
{
  Audio audio;
#ifdef __ANDROID__
  std::string prefix = "audios/";
#else
  std::string prefix = "assets/audios/";
#endif

  SDL_RWops *rw = SDL_RWFromFile((prefix + filePath + ".png").c_str(), "rb");

  if (rw == nullptr) {
    Logger::getInstance()->log(LogLevel::ERROR, "Unable to load audio %s! Error: %s", filePath.c_str(), SDL_GetError());
    return audio;
  }

  Mix_Chunk *chunk = Mix_LoadWAV_RW(rw, 1);
  if (chunk == nullptr) {
    Logger::getInstance()->log(
      LogLevel::ERROR, "Unable to parse audio %s! Error: %s", filePath.c_str(), Mix_GetError());
    return audio;
  }

  audio.Asign(chunk);
  SDL_RWclose(rw);

  return audio;
}
}// namespace Anterle
