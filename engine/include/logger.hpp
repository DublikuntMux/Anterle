#pragma once

#include <mutex>
#include <queue>
#include <string>
#include <thread>


namespace Anterle {
class Logger
{
public:
  Logger();
  ~Logger();

  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;
  Logger(Logger &&) = delete;
  Logger &operator=(Logger &&) = delete;

  static std::unique_ptr<Logger> &getInstance();

  void log(const char *format, ...);

private:
  void workerThread();

private:
  static std::unique_ptr<Logger> _instance;

  std::thread _worker;
  std::mutex _mtx;
  std::queue<std::string> _logQueue;
  std::string _logFileName;
  bool _running = true;
};
}// namespace Anterle