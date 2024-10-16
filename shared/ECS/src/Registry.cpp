#include "Registry.hpp"
#include "Entity.hpp"
#include "Logger.hpp"
#include "Player.hpp"
#include <format>
#include <set>

namespace ecs {
    Registry::AlreadyExist::AlreadyExist(uint64_t id) :
        _message(std::format("Entity with id ({0}) already exist", id)) {}

    const char *Registry::AlreadyExist::what() const noexcept {
        return this->_message.c_str();
    }

    Registry::ComponentNotFound::ComponentNotFound(const std::string& componentName) :
        _message(std::format("Cannot find component ({0})", componentName)) {}

    const char *Registry::ComponentNotFound::what() const noexcept {
        return this->_message.c_str();
    }

    /**
     *  \brief Generate a unique ID for an entity.
     */
    int Registry::_generateID() {
        std::lock_guard<std::mutex> lock(this->_mutex);
        int id = 1;
        while (std::ranges::any_of(this->_entities.begin(), this->_entities.end(), [id](auto& item) { return item.getID() == id; })) {
            id += 1;
        }
        return id;
    }

    /**
     *  \brief Create a registry with a unique ID.
     */
    Registry::Registry(uint8_t id) :
        _id(id) {}

    /**
     *  \brief Get all entities of the registry.
     */
    std::set<Entity> Registry::getEntities() {
        std::lock_guard<std::mutex> lock(this->_mutex);
        return _entities;
    }

    /**
     *  \brief Get number of components of an entity.
     */
    std::size_t Registry::getNoComponent(const Entity& entity) {
        std::lock_guard<std::mutex> lock(this->_mutex);
        std::size_t noComponent = 0;
        for (auto& component: this->_components) {
            if (component.second.contains(entity)) {
                noComponent += 1;
            }
        }
        return noComponent;
    }

    /**
     *  \brief Remove all components of an entity.
     */
    void Registry::reset(const Entity& entity) {
        std::lock_guard<std::mutex> lock(this->_mutex);
        for (auto& component: this->_components) {
            component.second.erase(entity);
        }
    }

    /**
     *  \brief Remove all entities and components.
     */
    void Registry::resetAll() {
        std::lock_guard<std::mutex> lock(this->_mutex);
        std::erase_if(this->_components, [this](const auto& item) { return !(item.first == typeid(ecs::component::Player)); });
        std::erase_if(this->_entities, [this](const auto& item) { return getNoComponent(item) == 0; });
    }

    /**
     *  \brief Remove an entity and its components.
     */
    void Registry::removeEntity(const Entity& entity) {
        std::lock_guard<std::mutex> lock(this->_mutex);
        _entities.erase(entity);
        for (auto& [type, componentMap]: _components) {
            componentMap.erase(entity);
        }
    }

    /**
     *  \brief Check if an entity is present in registry.
     */
    bool Registry::isEntityValid(const Entity& entity) {
        std::lock_guard<std::mutex> lock(this->_mutex);
        return _entities.contains(entity);
    }
}
