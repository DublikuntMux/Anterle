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

  GameLevel() = default;

  void Load(std::string name);
  bool IsCompleted();

private:
  static std::string FindByKey(std::string key, const std::string *text);
};
