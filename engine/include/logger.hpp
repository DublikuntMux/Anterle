#pragma once

#include <mutex>
#include <queue>
#include <string>
#include <thread>


namespace Anterle {
class Logger
{
public:
  static Logger *getInstance();

  ~Logger();

  void log(const char *format, ...);

private:
  Logger();
  void workerThread();

private:
  static Logger *instance;
  std::thread worker;
  std::mutex mtx;
  std::queue<std::string> logQueue;
  std::string logFileName;
  bool running = true;
};
}// namespace Anterle