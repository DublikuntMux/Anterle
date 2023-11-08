#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "component/component.hpp"

namespace Anterle {
class GameObject
{
public:
  glm::vec2 Position;
  float Rotation;
  bool Destroyed;

  GameObject();
  GameObject(glm::vec2 pos);

  void Update();
  void FixedUpdate();
  void ProcessInput();

  void addComponent(Component *component);

protected:
  std::vector<Component *> p_components;
};
}// namespace Anterle
