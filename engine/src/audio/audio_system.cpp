#include <string>
#include <unordered_map>

#include <miniaudio.h>

#include "audio/audio_system.hpp"

namespace Anterle {
AudioSystem::AudioSystem()
{
  ma_context_init(NULL, 0, NULL, &context);
  ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format = ma_format_f32;
  deviceConfig.playback.channels = 2;
  deviceConfig.sampleRate = 44100;
  deviceConfig.pUserData = this;

  if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
    throw AudioSystemException("Failed to initialize audio device!");
  }

  if (ma_device_start(&device) != MA_SUCCESS) { throw AudioSystemException("Failed to start audio device!"); }
}

AudioSystem::~AudioSystem()
{
  ma_device_uninit(&device);
  ma_context_uninit(&context);
}

void AudioSystem::createChannel(const std::string &channelName, const char *filePath)
{
  if (channels.find(channelName) == channels.end()) {
    ma_decoder decoder;
    if (ma_decoder_init_file(filePath, NULL, &decoder) == MA_SUCCESS) {
      channels[channelName] = { decoder, false, false, false, 1.0f };
    } else {
      throw AudioSystemException("Failed to load sound: " + std::string(filePath));
    }
  } else {
    throw AudioSystemException("Channel already exists: " + channelName);
  }
}

void AudioSystem::setSound(const std::string &channelName, const char *filePath)
{
  if (channels.find(channelName) != channels.end()) {
    ma_decoder decoder;
    if (ma_decoder_init_file(filePath, NULL, &decoder) == MA_SUCCESS) {
      ma_decoder_uninit(&channels[channelName].decoder);
      channels[channelName].decoder = decoder;
    } else {
      throw AudioSystemException("Failed to load sound: " + std::string(filePath));
    }
  } else {
    throw AudioSystemException("Channel does not exist: " + channelName);
  }
}

void AudioSystem::removeChannel(const std::string &channelName)
{
  auto it = channels.find(channelName);
  if (it != channels.end()) {
    ma_decoder_uninit(&it->second.decoder);
    channels.erase(it);
  } else {
    throw AudioSystemException("Channel does not exist: " + channelName);
  }
}

float AudioSystem::getChannelVolume(const std::string &channelName) const
{
  if (channels.find(channelName) != channels.end()) { return channels.at(channelName).volume; }
  return -1.0f;
}

bool AudioSystem::isChannelLooped(const std::string &channelName) const
{
  if (channels.find(channelName) != channels.end()) { return channels.at(channelName).isLooping; }
  return false;
}

bool AudioSystem::isChannelPaused(const std::string &channelName) const
{
  if (channels.find(channelName) != channels.end()) { return channels.at(channelName).isPaused; }
  return false;
}

void AudioSystem::play(const std::string &channelName)
{
  if (channels.find(channelName) != channels.end()) {
    Channel &channel = channels[channelName];
    if (!channel.isPlaying && !channel.isPaused) { ma_decoder_seek_to_pcm_frame(&channel.decoder, 0); }
    channel.isPlaying = true;
    channel.isPaused = false;
  }
}

void AudioSystem::stop(const std::string &channelName)
{
  if (channels.find(channelName) != channels.end()) {
    Channel &channel = channels[channelName];
    channel.isPlaying = false;
    channel.isPaused = false;
  }
}

void AudioSystem::pause(const std::string &channelName)
{
  if (channels.find(channelName) != channels.end()) {
    Channel &channel = channels[channelName];
    channel.isPaused = true;
  }
}

void AudioSystem::resume(const std::string &channelName)
{
  if (channels.find(channelName) != channels.end()) {
    Channel &channel = channels[channelName];
    if (!channel.isPlaying) { ma_decoder_seek_to_pcm_frame(&channel.decoder, 0); }
    channel.isPlaying = true;
    channel.isPaused = false;
  }
}

void AudioSystem::loop(const std::string &channelName, bool enable)
{
  if (channels.find(channelName) != channels.end()) { channels[channelName].isLooping = enable; }
}

void AudioSystem::setVolume(const std::string &channelName, float volume)
{
  if (channels.find(channelName) != channels.end()) { channels[channelName].volume = volume; }
}

void AudioSystem::update()
{
  for (auto &channel : channels) {
    Channel &ch = channel.second;
    if (ch.isPlaying && !ch.isPaused) {
      ma_decoder_seek_to_pcm_frame(&ch.decoder, 0);
      float volume = ch.volume;
      if (ma_decoder_read_pcm_frames(&ch.decoder, buffer, bufferSize, 0) == 0) {
        if (ch.isLooping) {
          ma_decoder_seek_to_pcm_frame(&ch.decoder, 0);
        } else {
          ch.isPlaying = false;
        }
      }

      for (int i = 0; i < bufferSize; ++i) { buffer[i] *= volume; }
    } else {
      memset(buffer, 0, sizeof(buffer));
    }
  }
}
}// namespace Anterle
