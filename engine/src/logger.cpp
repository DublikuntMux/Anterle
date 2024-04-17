#include <chrono>
#include <cstdarg>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>


#include "logger.hpp"

namespace Anterle {
std::unique_ptr<Logger> Logger::_instance = nullptr;

Logger::Logger()
{
  std::filesystem::path logDir("log");
  if (!std::filesystem::exists(logDir)) {
    if (!std::filesystem::create_directory(logDir)) {
      std::cerr << "Failed to create log directory: " << logDir << std::endl;
      return;
    }
  }

  auto now = std::chrono::system_clock::now();
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);
  std::tm now_tm{};
  localtime_s(&now_tm, &now_c);
  std::stringstream ss;
  ss << "log/" << std::put_time(&now_tm, "%Y-%m-%d_%H-%M-%S") << ".log";
  _logFileName = ss.str();

  _worker = std::thread(&Logger::workerThread, this);
}

Logger::~Logger()
{
  {
    std::unique_lock<std::mutex> lock(_mtx);
    _running = false;
  }
  if (_worker.joinable()) { _worker.join(); }
}

void Logger::workerThread()
{
  std::ofstream logFile(_logFileName, std::ios_base::app);
  if (!logFile.is_open()) {
    std::cerr << "Failed to open log file: " << _logFileName << std::endl;
    return;
  }

  while (true) {
    std::string message;
    {
      std::unique_lock<std::mutex> lock(_mtx);
      if (!_logQueue.empty()) {
        message = _logQueue.front();
        _logQueue.pop();
      }
      if (!_running && _logQueue.empty()) { break; }
    }
    if (!message.empty()) {
      logFile << message << std::endl;
      std::cout << message << std::endl;
    }
  }
  logFile.close();
}

std::unique_ptr<Logger> &Logger::getInstance()
{
  if (!_instance) { _instance = std::make_unique<Logger>(); }
  return _instance;
}

void Logger::log(const char *format, ...)
{
  va_list args = nullptr;
  va_start(args, format);
  int size = vsnprintf(nullptr, 0, format, args) + 1;
  std::string buffer(size, '\0');
  vsnprintf(&buffer[0], size, format, args);
  va_end(args);

  auto now = std::chrono::system_clock::now();
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);
  std::tm now_tm{};
  localtime_s(&now_tm, &now_c);
  std::stringstream ss;
  ss << "[" << std::put_time(&now_tm, "%H:%M:%S") << "] " << ": " << buffer;

  std::unique_lock<std::mutex> lock(_mtx);
  _logQueue.push(ss.str());
}
}// namespace Anterle
