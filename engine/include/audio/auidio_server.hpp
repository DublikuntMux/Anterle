#pragma once

#include <array>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include <miniaudio.h>

namespace Anterle {
struct Channel
{
  ma_decoder decoder;
  bool isPlaying;
  bool isLooping;
  bool isPaused;
  float volume;
};

class AudioException : public std::runtime_error
{
public:
  AudioException(const std::string &message) : std::runtime_error(message) {}
};

class AudioSystem
{
public:
  AudioSystem();
  ~AudioSystem();

  void createChannel(const std::string &channelName, const char *filePath);

  void setSound(const std::string &channelName, const char *filePath);
  void removeChannel(const std::string &channelName);
  void update();

  void play(const std::string &channelName);
  void stop(const std::string &channelName);
  void pause(const std::string &channelName);
  void resume(const std::string &channelName);
  void loop(const std::string &channelName, bool enable);
  void setVolume(const std::string &channelName, float volume);

  [[nodiscard]] float getChannelVolume(const std::string &channelName) const;
  [[nodiscard]] bool isChannelLooped(const std::string &channelName) const;
  [[nodiscard]] bool isChannelPaused(const std::string &channelName) const;

private:
  ma_context _context{};
  ma_device _device{};
  std::unordered_map<std::string, Channel> _channels;
  std::array<float, 4096> _buffer{};
};
}// namespace Anterle
