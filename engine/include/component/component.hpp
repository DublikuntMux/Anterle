#pragma once

namespace Anterle {
class GameObject;

class Component
{
public:
  Component(GameObject *parent);
  virtual ~Component();

  virtual void Update() = 0;
  virtual void FixedUpdate() = 0;
  virtual void ProcessInput() = 0;

  GameObject *getGameObject();

protected:
  GameObject *p_parent;
};
}// namespace Anterle
