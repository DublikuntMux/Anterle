#pragma once

#include "backend/base.hpp"
#include <memory>
class Game
{
public:
  Game(const char *window_name);
  virtual ~Game();

  virtual void Update(double deltaTime);
  void Start();

private:
  std::unique_ptr<BaseBackend> backend;
};
