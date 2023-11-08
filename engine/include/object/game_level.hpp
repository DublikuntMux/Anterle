#pragma once

#include <mutex>
#include <queue>
#include <string>
#include <vector>

#include "object/game_object.hpp"

namespace Anterle {
class GameLevel
{
public:
  GameLevel(std::string name);

  void Update();
  void FixedUpdate();
  void ProcessInput();

  void AddObject(GameObject* object);
  bool IsCompleted();
  void Reset();
  
protected:
std::vector<GameObject*> p_objects;

private:
  void ParallelTask(void (GameObject::*func)());
};

class ConcurrentQueue
{
public:
  void push(GameObject *comp);
  GameObject *pop();

private:
  std::queue<GameObject *> queue;
  std::mutex mtx;
  std::condition_variable cv;
};
}// namespace Anterle
