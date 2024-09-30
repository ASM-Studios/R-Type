#pragma once

#include "Entity.hpp"
#include "Registry.hpp"
#include <optional>

namespace ecs {
    template <typename... Components>
    Entity Registry::createEntity() {
        Entity entity(_maxId++);
        _entities.insert(entity);
        (Registry::addComponent<Components>(entity), ...);
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
        return _components[typeid(Component)].find(entity) != _components[typeid(Component)].end();
    }

    template <typename Component>
    std::optional<std::reference_wrapper<Component>> Registry::getComponent(const Entity& entity) {
        if (!contains<Component>(entity)) {
            return std::nullopt;
        }
        return std::any_cast<Component&>(_components[typeid(Component)][entity]);
    }
}
