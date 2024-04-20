#include <SDL.h>
#include <SDL_mixer.h>

#include "logger.hpp"

#include "audio/auidio_server.hpp"

namespace Anterle {
AudioChannel::AudioChannel(int channel) : channel(channel) {}

void AudioChannel::play(Audio audio, int loops) { Mix_PlayChannel(channel, (Mix_Chunk *)audio.chunk, loops); }
void AudioChannel::pause() { Mix_Pause(channel); }
void AudioChannel::resume() { Mix_Resume(channel); }
void AudioChannel::stop() { Mix_HaltChannel(channel); }

void AudioChannel::setVolume(int volume) { Mix_Volume(channel, volume); }
void AudioChannel::setSpeed(int speed) { Mix_SetPanning(channel, speed, 255 - speed); }

AudioSystem::AudioSystem()
{
  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0) {
    Logger::getInstance()->log(LogLevel::CRITICAL, "Failde to init auido: %s", Mix_GetError());
  }
}

AudioSystem::~AudioSystem() { Mix_CloseAudio(); }

AudioChannel *AudioSystem::getChannel(int channel)
{
  if (channel < 0 || channel >= Mix_AllocateChannels(-1)) {
    Logger::getInstance()->log(LogLevel::ERROR, "Invalid channel number: %i", channel);
    return nullptr;
  }
  return &channels[channel];
}

void AudioSystem::play(int channel, Audio audio, int loops) { getChannel(channel)->play(audio, loops); }
void AudioSystem::pause(int channel) { getChannel(channel)->pause(); }
void AudioSystem::resume(int channel) { getChannel(channel)->resume(); }
void AudioSystem::stop(int channel) { getChannel(channel)->stop(); }

void AudioSystem::pauseAll()
{
  for (auto &channel : channels) { channel.pause(); }
}
void AudioSystem::resumeAll()
{
  for (auto &channel : channels) { channel.resume(); }
}
void AudioSystem::stopAll()
{
  for (auto &channel : channels) { channel.stop(); }
}

void AudioSystem::setVolume(int channel, int volume) { getChannel(channel)->setVolume(volume); }
void AudioSystem::setSpeed(int channel, int speed) { getChannel(channel)->setSpeed(speed); }

void AudioSystem::setVolumeAll(int volume)
{
  for (auto &channel : channels) { channel.setVolume(volume); }
}
void AudioSystem::setSpeedAll(int speed)
{
  for (auto &channel : channels) { channel.setSpeed(speed); }
}
}// namespace Anterle
