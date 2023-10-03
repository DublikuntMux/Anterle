#pragma once

#include <string>
namespace Anterle {
class GameLevel
{
public:
  std::string Name;

  GameLevel();

  void Load(std::string name);
  bool IsCompleted();
};
}// namespace Anterle
