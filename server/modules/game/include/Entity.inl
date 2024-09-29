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
    void Entity::setComponent(Component component) {
        Registry::setComponent(*this, component);
    }

    template <typename Component>
    bool Entity::contains() const {
        return Registry::contains<Component>(*this);
    }

    template <typename Component>
    std::optional<std::reference_wrapper<Component>> Entity::getComponent() {
        return Registry::getComponent<Component>(*this);
    }

    template <typename Component>
    std::optional<Component> Entity::getComponent() const {
        return Registry::getComponent<Component>(*this);
    }
}
