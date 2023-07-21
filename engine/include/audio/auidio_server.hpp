#pragma once

#include <string>
#include <unordered_map>

#include <miniaudio.h>

namespace Anterle {
class AudioServer
{
public:
  AudioServer();
  ~AudioServer();

  void PlaySound(std::string file_path, bool loop = false);
  void SetVolume(float volume);

private:
  ma_engine m_engine;
};
}// namespace Anterle
