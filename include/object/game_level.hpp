#pragma once

#include <cstdint>
#include <string>
#include <vector>

class GameLevel {
public:
  std::vector<std::string> Lines;
  std::string Name;
  std::string Description;

  GameLevel() {}

  void Load(std::string name);
  bool IsCompleted();

private:
  void init(std::vector<std::string> lines);
};
