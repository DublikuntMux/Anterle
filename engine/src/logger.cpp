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
Logger *Logger::instance = nullptr;

Logger::Logger() : running(true)
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
  std::tm now_tm;
  localtime_s(&now_tm, &now_c);
  std::stringstream ss;
  ss << "log/" << std::put_time(&now_tm, "%Y-%m-%d_%H-%M-%S") << ".log";
  logFileName = ss.str();

  worker = std::thread(&Logger::workerThread, this);
}

Logger::~Logger()
{
  {
    std::unique_lock<std::mutex> lock(mtx);
    running = false;
  }
  if (worker.joinable()) { worker.join(); }
}

void Logger::workerThread()
{
  std::ofstream logFile(logFileName, std::ios_base::app);
  if (!logFile.is_open()) {
    std::cerr << "Failed to open log file: " << logFileName << std::endl;
    return;
  }

  while (true) {
    std::string message;
    {
      std::unique_lock<std::mutex> lock(mtx);
      if (!logQueue.empty()) {
        message = logQueue.front();
        logQueue.pop();
      }
      if (!running && logQueue.empty()) { break; }
    }
    if (!message.empty()) {
      logFile << message << std::endl;
      std::cout << message << std::endl;
    }
  }
  logFile.close();
}

Logger *Logger::getInstance()
{
  if (!instance) { instance = new Logger(); }
  return instance;
}

void Logger::log(const char *format, ...)
{
  char buffer[1024];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);

  auto now = std::chrono::system_clock::now();
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);
  std::tm now_tm;
  localtime_s(&now_tm, &now_c);
  std::stringstream ss;
  ss << "[" << std::put_time(&now_tm, "%H:%M:%S") << "] " << ": " << buffer;
  
  std::unique_lock<std::mutex> lock(mtx);
  logQueue.push(ss.str());
}
}// namespace Anterle
