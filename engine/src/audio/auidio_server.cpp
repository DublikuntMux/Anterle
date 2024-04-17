#include <string>
#include <unordered_map>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include "audio/auidio_server.hpp"

namespace Anterle {
AudioSystem::AudioSystem()
{
  ma_context_init(nullptr, 0, nullptr, &_context);
  ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format = ma_format_f32;
  deviceConfig.playback.channels = 2;
  deviceConfig.sampleRate = 44100;
  deviceConfig.pUserData = this;

  if (ma_device_init(nullptr, &deviceConfig, &_device) != MA_SUCCESS) {
    throw AudioException("Failed to initialize audio device!");
  }

  if (ma_device_start(&_device) != MA_SUCCESS) { throw AudioException("Failed to start audio device!"); }
}

AudioSystem::~AudioSystem()
{
  ma_device_uninit(&_device);
  ma_context_uninit(&_context);
}

void AudioSystem::createChannel(const std::string &channelName, const char *filePath)
{
  if (_channels.find(channelName) == _channels.end()) {
    ma_decoder decoder;
    if (ma_decoder_init_file(filePath, nullptr, &decoder) == MA_SUCCESS) {
      _channels[channelName] = { decoder, false, false, false, 1.0f };
    } else {
      throw AudioException("Failed to load sound: " + std::string(filePath));
    }
  } else {
    throw AudioException("Channel already exists: " + channelName);
  }
}

void AudioSystem::setSound(const std::string &channelName, const char *filePath)
{
  if (_channels.find(channelName) != _channels.end()) {
    ma_decoder decoder;
    if (ma_decoder_init_file(filePath, nullptr, &decoder) == MA_SUCCESS) {
      ma_decoder_uninit(&_channels[channelName].decoder);
      _channels[channelName].decoder = decoder;
    } else {
      throw AudioException("Failed to load sound: " + std::string(filePath));
    }
  } else {
    throw AudioException("Channel does not exist: " + channelName);
  }
}

void AudioSystem::removeChannel(const std::string &channelName)
{
  auto it = _channels.find(channelName);
  if (it != _channels.end()) {
    ma_decoder_uninit(&it->second.decoder);
    _channels.erase(it);
  } else {
    throw AudioException("Channel does not exist: " + channelName);
  }
}

float AudioSystem::getChannelVolume(const std::string &channelName) const
{
  if (_channels.find(channelName) != _channels.end()) { return _channels.at(channelName).volume; }
  return -1.0f;
}

bool AudioSystem::isChannelLooped(const std::string &channelName) const
{
  if (_channels.find(channelName) != _channels.end()) { return _channels.at(channelName).isLooping; }
  return false;
}

bool AudioSystem::isChannelPaused(const std::string &channelName) const
{
  if (_channels.find(channelName) != _channels.end()) { return _channels.at(channelName).isPaused; }
  return false;
}

void AudioSystem::play(const std::string &channelName)
{
  if (_channels.find(channelName) != _channels.end()) {
    Channel &channel = _channels[channelName];
    if (!channel.isPlaying && !channel.isPaused) { ma_decoder_seek_to_pcm_frame(&channel.decoder, 0); }
    channel.isPlaying = true;
    channel.isPaused = false;
  }
}

void AudioSystem::stop(const std::string &channelName)
{
  if (_channels.find(channelName) != _channels.end()) {
    Channel &channel = _channels[channelName];
    channel.isPlaying = false;
    channel.isPaused = false;
  }
}

void AudioSystem::pause(const std::string &channelName)
{
  if (_channels.find(channelName) != _channels.end()) {
    Channel &channel = _channels[channelName];
    channel.isPaused = true;
  }
}

void AudioSystem::resume(const std::string &channelName)
{
  if (_channels.find(channelName) != _channels.end()) {
    Channel &channel = _channels[channelName];
    if (!channel.isPlaying) { ma_decoder_seek_to_pcm_frame(&channel.decoder, 0); }
    channel.isPlaying = true;
    channel.isPaused = false;
  }
}

void AudioSystem::loop(const std::string &channelName, bool enable)
{
  if (_channels.find(channelName) != _channels.end()) { _channels[channelName].isLooping = enable; }
}

void AudioSystem::setVolume(const std::string &channelName, float volume)
{
  if (_channels.find(channelName) != _channels.end()) { _channels[channelName].volume = volume; }
}

void AudioSystem::update()
{
  for (auto &channel : _channels) {
    Channel &ch = channel.second;
    if (ch.isPlaying && !ch.isPaused) {
      ma_decoder_seek_to_pcm_frame(&ch.decoder, 0);
      float volume = ch.volume;
      if (ma_decoder_read_pcm_frames(&ch.decoder, _buffer.data(), _buffer.size(), nullptr) == 0) {
        if (ch.isLooping) {
          ma_decoder_seek_to_pcm_frame(&ch.decoder, 0);
        } else {
          ch.isPlaying = false;
        }
      }

      for (float &i : _buffer) { i *= volume; }
    } else {
      memset(_buffer.data(), 0, sizeof(_buffer));
    }
  }
}
}// namespace Anterle
