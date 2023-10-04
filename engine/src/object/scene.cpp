#include <omp.h>

#include "object/game_object.hpp"
#include "object/scene.hpp"

void Scene::Update(double deltaTime)
{
#pragma omp parallel for
  for (const auto &object : game_objects) { object->Update(deltaTime); }
}

void Scene::OnSpawn()
{
#pragma omp parallel for
  for (const auto &object : game_objects) { object->OnSpawn(); }
}

void Scene::OnDespawn()
{
#pragma omp parallel for
  for (const auto &object : game_objects) { object->OnDespawn(); }
}

void Scene::addGameObject(std::shared_ptr<GameObject> object) { game_objects.push_back(object); }
