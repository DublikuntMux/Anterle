#pragma once

#include <memory>

#include "object/object.hpp"

class GameObject;

class Component : public Object
{
public:
  virtual void Update(double deltaTime);
  virtual void OnSpawn();
  virtual void OnDespawn();

  std::shared_ptr<GameObject> getGameObject();

protected:
  std::weak_ptr<GameObject> gameObject;
};
