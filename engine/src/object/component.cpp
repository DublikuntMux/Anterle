#include "object/component.hpp"
#include "object/game_object.hpp"

namespace Anterle {
std::shared_ptr<GameObject> Component::getGameObject() { return gameObject.lock(); }
}// namespace Anterle
