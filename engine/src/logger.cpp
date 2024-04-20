#include <cstdarg>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include <SDL.h>

#include "logger.hpp"
#include "utils.hpp"

namespace Anterle {
std::unique_ptr<Logger> Logger::_instance = nullptr;

Logger::Logger()
{
  std::string baseFolder = SDL_GetPrefPath("Dublikunt", "Anterle");

  std::filesystem::path logDir(baseFolder + "/log");
  if (!std::filesystem::exists(logDir)) {
    if (!std::filesystem::create_directory(logDir)) {
      Logger::getInstance()->log(LogLevel::ERROR, "Failed to create log directory.");
      return;
    }
  }

  std::vector<std::filesystem::directory_entry> logFiles;
  for (const auto &entry : std::filesystem::directory_iterator(logDir)) {
    if (entry.path().extension() == ".log") { logFiles.push_back(entry); }
  }
  std::sort(logFiles.begin(), logFiles.end(), [](const auto &a, const auto &b) {
    return a.last_write_time() > b.last_write_time();
  });

  while (logFiles.size() > 5) {
    std::filesystem::remove(logFiles.back().path());
    logFiles.pop_back();
  }

  _logFileName = baseFolder + "log/" + Utils::CurrentTimeAsString() + ".log";

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

  while (_running) {
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

void Logger::log(LogLevel level, const char *format, ...)
{
  va_list args = nullptr;
  va_start(args, format);
  int size = vsnprintf(nullptr, 0, format, args) + 1;
  std::string buffer(size, '\0');
  vsnprintf(&buffer[0], size, format, args);
  va_end(args);

  std::string levelText;
  switch (level) {
  case LogLevel::INFO:
    levelText = "[Info]: ";
    break;
  case LogLevel::WARN:
    levelText = "[Warning]: ";
    break;
  case LogLevel::ERROR:
    levelText = "[Error]: ";
    break;
  case LogLevel::CRITICAL:
    levelText = "[Critical]: ";
    break;
  }

  auto now = std::chrono::system_clock::now();
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);
  std::tm now_tm{};
  localtime_s(&now_tm, &now_c);
  std::stringstream ss;
  ss << "[" << std::put_time(&now_tm, "%H:%M:%S") << "]: " << levelText << buffer;

  std::unique_lock<std::mutex> lock(_mtx);
  _logQueue.push(ss.str());

  if (level == LogLevel::CRITICAL) { abort(); }
}
}// namespace Anterle
