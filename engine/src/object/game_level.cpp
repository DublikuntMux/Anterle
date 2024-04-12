#include <string>

#include "object/game_level.hpp"
#include "object/game_object.hpp"

namespace Anterle {
GameLevel::GameLevel(std::string level_name) {}

void GameLevel::Update()
{
  if (p_objects.size() >= 3) {
    ParallelTask(&GameObject::Update);
  } else {
    for (GameObject *gobject : p_objects) { (gobject->*(&GameObject::Update))(); }
  }
}
void GameLevel::FixedUpdate()
{
  if (p_objects.size() >= 3) {
    ParallelTask(&GameObject::FixedUpdate);
  } else {
    for (GameObject *gobject : p_objects) { (gobject->*(&GameObject::FixedUpdate))(); }
  }
}
void GameLevel::ProcessInput()
{
  if (p_objects.size() >= 3) {
    ParallelTask(&GameObject::ProcessInput);
  } else {
    for (GameObject *gobject : p_objects) { (gobject->*(&GameObject::ProcessInput))(); }
  }
}

void GameLevel::AddObject(GameObject *object) { p_objects.emplace_back(object); }
bool GameLevel::IsCompleted() { return false; }
void GameLevel::Reset() {}

void GameLevel::ParallelTask(void (GameObject::*func)())
{
  ConcurrentQueue taskQueue;
  for (GameObject *gobject : p_objects) { taskQueue.push(gobject); }

  auto worker = [func, &taskQueue]() {
    while (true) {
      GameObject *gobject = taskQueue.pop();
      if (gobject == nullptr) break;
      (gobject->*func)();
    }
  };

  unsigned int numCores = std::thread::hardware_concurrency();
  unsigned int numThreads = (numCores <= 2) ? 2 : numCores - 1;

  std::vector<std::thread> threads;
  for (unsigned int i = 0; i < numThreads; ++i) { threads.emplace_back(worker); }

  for (auto &thread : threads) { thread.join(); }
}

void ConcurrentQueue::push(GameObject *gobject)
{
  std::lock_guard<std::mutex> lock(mtx);
  queue.push(gobject);
  cv.notify_one();
}

GameObject *ConcurrentQueue::pop()
{
  std::unique_lock<std::mutex> lock(mtx);
  while (queue.empty()) { cv.wait(lock); }
  GameObject *gobject = queue.front();
  queue.pop();
  return gobject;
}
}// namespace Anterle
