#include "component/component.hpp"

namespace Anterle {
Component::Component(GameObject *parent) : p_parent(parent) {}
Component::~Component() {}

GameObject *Component::getGameObject() { return p_parent; }
}// namespace Anterle
