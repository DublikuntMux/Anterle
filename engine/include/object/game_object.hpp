#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "object/object.hpp"

namespace Anterle {
class Component;

class GameObject : public Object
{
public:
  void Update(double deltaTime);
  void OnSpawn();
  void OnDespawn();

  void addComponent(std::shared_ptr<Component> component);

  void setPosition(const glm::vec3 &newPosition);
  void setRotation(const glm::quat &newRotation);

  const glm::vec3 &getPosition() const;
  const glm::quat &getRotation() const;

  glm::vec3 position;
  glm::quat rotation;

protected:
  std::vector<std::shared_ptr<Component>> components;
};
}// namespace Anterle
