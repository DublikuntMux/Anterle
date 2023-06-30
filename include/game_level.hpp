#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "game_object.hpp"

class GameLevel {
public:
  std::vector<GameObject> Objects;

  GameLevel() {}

  void Load(const char *file_name);
  bool IsCompleted();

private:
  void init(std::vector<std::string> lines);
};
