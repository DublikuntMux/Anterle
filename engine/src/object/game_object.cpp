#include "object/game_object.hpp"
#include "object/component.hpp"

namespace Anterle {
void GameObject::Update(double deltaTime)
{
#pragma omp parallel for
  for (const auto &component : components) { component->Update(deltaTime); }
}

void GameObject::OnSpawn()
{
#pragma omp parallel for
  for (const auto &component : components) { component->OnSpawn(); }
}

void GameObject::OnDespawn()
{
#pragma omp parallel for
  for (const auto &component : components) { component->OnDespawn(); }
}

void GameObject::addComponent(std::shared_ptr<Component> component) { components.push_back(component); }

void GameObject::setPosition(const glm::vec3 &newPosition) { position = newPosition; }

void GameObject::setRotation(const glm::quat &newRotation) { rotation = newRotation; }

const glm::vec3 &GameObject::getPosition() const { return position; }

const glm::quat &GameObject::getRotation() const { return rotation; }
}// namespace Anterle
