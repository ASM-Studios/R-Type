#pragma once

#include "Entity.hpp"
#include "Registry.hpp"

namespace ecs {
    template <typename... Components>
    Entity Registry::createEntity() {
        Entity entity(_maxId++);
        _entities.insert(entity);
        (Registry::addComponent<Components>(entity), ...);
        return entity;
    }

    template <typename Component>
    void Registry::addComponent(Entity entity) {
        _components[typeid(Component)][entity] = Component();
    }

    template <typename... Components>
    void Registry::addComponents(Entity entity) {
        (addComponent<Components>(entity), ...);
    }

    template <typename Component>
    bool Registry::contains(Entity entity) {
        return _components[typeid(Component)].find(entity) != _components[typeid(Component)].end();
    }

    template <typename Component>
    Component& Registry::getComponent(Entity entity) {
        if (!contains<Component>(entity)) {
            throw NoComponent(typeid(Component));
        }
        return std::any_cast<Component&>(_components[typeid(Component)][entity]);
    }
}
