#pragma once

#include "Entity.hpp"
#include "Registry.hpp"

namespace ecs {
    template <typename... Components>
    Entity Registry::createEntity() {
        return this->createEntity(this->_generateID());
    }

    template <typename... Components>
    Entity Registry::createEntity(int id) {
        for (auto entity: this->_entities) {
            if (entity.getID() == id) {
                throw AlreadyExist(id);
            }
        }
        const Entity entity(id, *this);
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
        (addComponent<Components>(entity), ...);
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
