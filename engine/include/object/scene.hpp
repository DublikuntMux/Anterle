#pragma once

#include <memory>
#include <vector>

#include "object/object.hpp"

namespace Anterle {
class GameObject;

class Scene : public Object
{
public:
  void Update(double deltaTime);
  void OnSpawn();
  void OnDespawn();

  void addGameObject(std::shared_ptr<GameObject> object);

protected:
  std::vector<std::shared_ptr<GameObject>> game_objects;
};
}// namespace Anterle
