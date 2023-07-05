#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>

#include "object/game_level.hpp"

void GameLevel::Load(std::string name) {
  std::vector<std::pair<std::string, std::string>> speech;

  std::string delimSpeech = " | ";
  std::string delimName = "Name: ";
  std::string delimDescription = "Description: ";

  std::string file_name = "resources/levels/" + name + ".lvl";

  std::ifstream file(file_name);
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      if (!(&line[0] == "#") || line != "" || line != " ") {
        this->Name = FindByKey(delimName, &line);
        this->Description = FindByKey(delimDescription, &line);
        {
          size_t end = line.find(delimSpeech);
          while (end != std::string::npos) {
            std::string name = line.substr(0U, end - 0U);
            uint64_t start = end + delimSpeech.length();
            end = line.find(delimSpeech, start);
            speech.emplace_back(std::make_pair(name, line.substr(start, end)));
          }
        }
      }
    }
  } else {
    spdlog::error("Failed to load level: {0}", file_name);
  }

  this->Speech = speech;

  file.close();
}

bool GameLevel::IsCompleted() { return false; }

std::string GameLevel::FindByKey(std::string key, const std::string *text) {
  std::string returnTexr;

  size_t end = text->find(key);
  while (end != std::string::npos) {
    uint64_t start = end + key.length();
    end = text->find(key, start);
    returnTexr = text->substr(start, end);
  }
  return returnTexr;
}
