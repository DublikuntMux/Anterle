#include <string>

#include <loguru.hpp>

#include <miniaudio.h>

#include "audio/auidio_server.hpp"

namespace Anterle {
AudioServer::AudioServer()
{
  ma_engine_config config;
  config = ma_engine_config_init();
  config.channels = 2;
  config.sampleRate = 48000;

  ma_result result = ma_engine_init(&config, &m_engine);
  if (result != MA_SUCCESS) { LOG_F(ERROR, "Failed to init audio engine."); }
}

AudioServer::~AudioServer() { ma_engine_uninit(&m_engine); }

void AudioServer::PlaySound(std::string file_path, bool loop)
{
  if (loop) {
    LOG_F(WARNING, "Loop sound not supported et.");
  } else {
    ma_engine_play_sound(&m_engine, file_path.c_str(), NULL);
  }
}

void AudioServer::SetVolume(float volume) { ma_engine_set_volume(&m_engine, volume); }
}// namespace Anterle
