#include "component/component.hpp"
#include "object/game_object.hpp"

namespace Anterle {
GameObject::GameObject() : Position(0.0f, 0.0f), Rotation(0.0f), Destroyed(false) {}

GameObject::GameObject(glm::vec2 pos) : Position(pos), Rotation(0.0f), Destroyed(false) {}

void GameObject::Update() {
        for (Component *comp : p_components) {
            (comp->*(&Component::Update))();
        }
}

void GameObject::FixedUpdate() {
        for (Component *comp : p_components) {
            (comp->*(&Component::FixedUpdate))();
        }
}

void GameObject::ProcessInput() {
        for (Component *comp : p_components) {
            (comp->*(&Component::ProcessInput))();
        }
}
}// namespace Anterle
