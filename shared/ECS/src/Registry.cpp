#include "Registry.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include <format>
#include <set>

namespace ecs {
    Registry::AlreadyExist::AlreadyExist(int id) :
        _message(std::format("Entity with id ({0}) already exist", id)) {}

    const char *Registry::AlreadyExist::what() const noexcept {
        return this->_message.c_str();
    }

    Registry::ComponentNotFound::ComponentNotFound(const std::string& componentName) :
        _message(std::format("Cannot find component ({0})", componentName)) {}

    const char *Registry::ComponentNotFound::what() const noexcept {
        return this->_message.c_str();
    }

    int Registry::_generateID() {
        int id = 1;
        while (std::ranges::any_of(this->_entities.begin(), this->_entities.end(), [id](auto& item) { return item.getID() == id; })) {
            id += 1;
        }
        return id;
    }

    Registry::Registry(uint8_t id) :
        _id(id) {}

    std::set<Entity> Registry::getEntities() {
        return _entities;
    }

    std::size_t Registry::getNoComponent(const Entity& entity) {
        std::size_t noComponent = 0;
        for (auto& component: this->_components) {
            if (component.second.contains(entity)) {
                noComponent += 1;
            }
        }
        return noComponent;
    }

    void Registry::display(const Entity& entity) {
        std::cout << entity.getID() << ":" << std::endl;
        for (auto& component: this->_components) {
            if (component.second.contains(entity)) {
                std::cout << "\t" << component.second.at(entity).type().name() << std::endl;
            }
        }
    }

    void Registry::displayAll() {
        for (auto& entity: this->_entities) {
            this->display(entity);
        }
    }

    void Registry::reset(const Entity& entity) {
        for (auto& component: this->_components) {
            component.second.erase(entity);
        }
    }

    void Registry::resetAll() {
        std::erase_if(this->_components, [this](const auto& item) { return !(item.first == typeid(ecs::component::Player)); });
        std::erase_if(this->_entities, [this](const auto& item) { return getNoComponent(item) == 0; });
    }

    void Registry::removeEntity(const Entity& entity) {
        _entities.erase(entity);
        for (auto& [type, componentMap] : _components) {
            componentMap.erase(entity);
        }
    }

    bool Registry::isEntityValid(const Entity& entity) const {
        return _entities.contains(entity);
    }
}
