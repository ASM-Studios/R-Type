#pragma once

#include "Entity.hpp"
#include "Registry.hpp"
#include <unordered_map>

namespace ecs {
    template <typename... Components>
    Entity Registry::createEntity() {
        int id = this->_generateID();
        return this->createEntity(id);
    }

    template <typename... Components>
    Entity Registry::createEntity(uint64_t id) {
        std::lock_guard<std::mutex> lock(this->_mutex);
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
    std::unordered_map<Entity, Component> Registry::getEntities() {
        std::lock_guard<std::mutex> lock(this->_mutex);
        std::unordered_map<Entity, Component> castedMap;
        for (const auto& [entity, component]: _components[typeid(Component)]) {
            castedMap.insert({entity, std::any_cast<Component>(component)});
        }
        return castedMap;
    }

    template <typename Component>
    void Registry::addComponent(const Entity& entity) {
        std::lock_guard<std::mutex> lock(this->_mutex);
        _components[typeid(Component)][entity] = Component();
    }

    template <typename... Components>
    void Registry::addComponents(const Entity& entity) {
        (addComponent<Components>(entity), ...);
    }

    template <typename Component>
    void Registry::setComponent(const Entity& entity, Component component) {
        std::lock_guard<std::mutex> lock(this->_mutex);
        _components[typeid(Component)][entity] = component;
    }

    template <typename Component>
    bool Registry::contains(const Entity& entity) {
        std::lock_guard<std::mutex> lock(this->_mutex);
        return _components[typeid(Component)].contains(entity);
    }

    template <typename Component>
    bool Registry::contains(const Entity& entity, const Component& component) {
        return this->contains<Component>(entity) && getComponent<Component>(entity) == component;
    }

    template <typename Component>
    Component& Registry::getComponent(const Entity& entity) {
        if (!contains<Component>(entity)) {
            throw ComponentNotFound(typeid(Component).name());
        }
        std::lock_guard<std::mutex> lock(this->_mutex);
        return std::any_cast<Component&>(_components[typeid(Component)][entity]);
    }
}
