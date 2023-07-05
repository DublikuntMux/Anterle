#pragma once

#include <cstdint>
#include <string>
#include <vector>

class GameLevel
{
public:
  std::vector<std::pair<std::string, std::string>> Speech;
  std::string Name;
  std::string Description;

  GameLevel() {}

  void Load(std::string name);
  bool IsCompleted();

private:
  std::string FindByKey(std::string key, const std::string *text);
};
