#include "object/component.hpp"
#include "object/game_object.hpp"

std::shared_ptr<GameObject> Component::getGameObject() { return gameObject.lock(); }
