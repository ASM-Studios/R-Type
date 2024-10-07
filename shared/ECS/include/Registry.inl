#pragma once

#include "Entity.hpp"
#include "Registry.hpp"

namespace ecs {
    template <typename... Components>
    Entity Registry::createEntity() {
        const Entity entity(_maxId++);
        _entities.insert(entity);
        Registry::addComponents<Components...>(entity);
        return entity;
    }

    template <typename Component>
    void Registry::addComponent(const Entity& entity) {
        _components[typeid(Component)][entity] = Component();
    }

    template <typename... Components>
    void Registry::addComponents(const Entity& entity) {
        (addComponent<Components>(entity, Components()), ...);
    }

    template <typename Component>
    void Registry::setComponent(const Entity& entity, Component component) {
        _components[typeid(Component)][entity] = component;
    }

    template <typename Component>
    bool Registry::contains(const Entity& entity) {
        return _components[typeid(Component)].contains(entity);
    }

    template <typename Component>
    bool Registry::contains(const Entity& entity, const Component& component) {
        return contains<Component>(entity) && getComponent<Component>(entity) == component;
    }

    template <typename Component>
    Component& Registry::getComponent(const Entity& entity) {
        if (!contains<Component>(entity)) {
            throw ComponentNotFound(typeid(Component).name());
        }
        return std::any_cast<Component&>(_components[typeid(Component)][entity]);
    }
}
