#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>

#include "object/game_level.hpp"

void GameLevel::Load(std::string name) {
  std::vector<std::string> lines;

  std::string file_name = "resources/levels/" + name + ".lvl";

  std::ifstream file(file_name);
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      lines.emplace_back(line);
    }
    file.close();
  } else
    spdlog::error("Failed to load level: {0}", file_name);

  GameLevel::init(lines);
}

bool GameLevel::IsCompleted() { return false; }

void GameLevel::init(std::vector<std::string> lines) {}
