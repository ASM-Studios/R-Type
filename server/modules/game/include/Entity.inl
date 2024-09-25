#pragma once

#include "Entity.hpp"
#include "Registry.hpp"

namespace ecs {
    template <typename Component>
    void Entity::addComponent() {
        Registry::addComponent<Component>(*this);
    }

    template <typename... Components>
    void Entity::addComponents() {
        Registry::addComponents<Components...>(*this);
    }

    template <typename Component>
    bool Entity::contains() const {
        return Registry::contains<Component>(*this);
    }

    template <typename Component>
    Component& Entity::getComponent() {
        return Registry::getComponent<Component>(*this);
    }
}
