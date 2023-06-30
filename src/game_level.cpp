#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>

#include "game_level.hpp"

void GameLevel::Load(std::string name) {
  std::vector<std::string> lines;

  std::string file_name = "resources/levels/" + name + ".lvl";

  std::string line;
  std::ifstream fileStream(file_name);
  if (fileStream.is_open()) {
    while (!fileStream.eof()) {
      getline(fileStream, line);
      lines.push_back(line);
    }
    fileStream.close();
  } else
    spdlog::error("Failed to load level: {0}", file_name);

  GameLevel::init(lines);
}

bool GameLevel::IsCompleted() { return false; }

void GameLevel::init(std::vector<std::string> lines) {}
