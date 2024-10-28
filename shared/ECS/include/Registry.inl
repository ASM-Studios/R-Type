#pragma once

#include "Entity.hpp"
#include "Registry.hpp"
#include <unordered_map>

namespace ecs {
    /**
     *  \brief Create an entity with a unique ID.
     */
    template <typename... Components>
    Entity Registry::createEntity() {
        int id = this->_generateID();
        return this->createEntity(id);
    }

    /**
     *  \brief Create an entity with a specific ID.
     */
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

    /**
     *  \brief Get all entities with component.
     */
    template <typename Component>
    std::unordered_map<Entity, Component> Registry::getEntities() {
        std::lock_guard<std::mutex> lock(this->_mutex);
        std::unordered_map<Entity, Component> castedMap;
        for (auto pair: _components[typeid(Component)]) {
            castedMap.insert({pair.first, std::any_cast<Component>(pair.second)});
        }
        return castedMap;
    }

    /**
     *  \brief Add a component to an entity.
     */
    template <typename Component>
    void Registry::addComponent(const Entity& entity) {
        std::lock_guard<std::mutex> lock(this->_mutex);
        _components[typeid(Component)][entity] = Component();
    }

    /**
     *  \brief Add multiple components to an entity.
     */
    template <typename... Components>
    void Registry::addComponents(const Entity& entity) {
        (addComponent<Components>(entity), ...);
    }

    /**
     *  \brief Set a component to an entity.
     *
     *  Set a component to an entity. If the entity already has a component of the same type, it will be replaced.
     *  If the entity does not have a component of the same type, it will be added.
     */
    template <typename Component>
    void Registry::setComponent(const Entity& entity, Component component) {
        std::lock_guard<std::mutex> lock(this->_mutex);
        _components[typeid(Component)][entity] = component;
    }

    /**
     *  \brief Check if an entity has a component.
     */
    template <typename Component>
    bool Registry::contains(const Entity& entity) {
        std::lock_guard<std::mutex> lock(this->_mutex);
        return _components[typeid(Component)].contains(entity);
    }

    /**
     *  \brief Check if an entity has a component with a specific value.
     */
    template <typename Component>
    bool Registry::contains(const Entity& entity, const Component& component) {
        return this->contains<Component>(entity) && getComponent<Component>(entity) == component;
    }

    /**
     *  \brief Get a component from an entity.
     *  \throws ComponentNotFound if the entity does not have the component.
     */
    template <typename Component>
    Component& Registry::getComponent(const Entity& entity) {
        if (!contains<Component>(entity)) {
            throw ComponentNotFound(typeid(Component).name());
        }
        std::lock_guard<std::mutex> lock(this->_mutex);
        return std::any_cast<Component&>(_components[typeid(Component)][entity]);
    }

    template <typename Component>
    std::vector<Component> Registry::getComponents() {
        std::vector<Component> components;

        for (const auto& [entity, component]: this->_components[typeid(Component)]) {
            components.push_back(std::any_cast<Component>(component));
        }
        return components;
    }
}
