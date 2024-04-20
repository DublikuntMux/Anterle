#pragma once

#include <vector>

#include "resource/audio.hpp"

namespace Anterle {
class AudioChannel
{
public:
  AudioChannel(int channel);

  void play(Audio audio, int loops = 0);
  void pause();
  void resume();
  void stop();
  void setVolume(int volume);
  void setSpeed(int speed);

private:
  int channel;
};

class AudioSystem
{
public:
  AudioSystem();
  ~AudioSystem();

  AudioSystem(const AudioSystem &) = default;
  AudioSystem &operator=(const AudioSystem &) = default;
  AudioSystem(AudioSystem &&) noexcept = default;
  AudioSystem &operator=(AudioSystem &&) noexcept = default;

  AudioChannel *getChannel(int channel);

  void play(int channel, Audio audio, int loops = 0);
  void pause(int channel);
  void resume(int channel);
  void stop(int channel);

  void pauseAll();
  void resumeAll();
  void stopAll();

  void setVolume(int channel, int volume);
  void setSpeed(int channel, int speed);

  void setVolumeAll(int volume);
  void setSpeedAll(int speed);

private:
  std::vector<AudioChannel> channels;
};
}// namespace Anterle
