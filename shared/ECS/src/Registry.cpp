#include "Registry.hpp"
#include "Entity.hpp"
#include <format>
#include <set>

namespace ecs {
    Registry::ComponentNotFound::ComponentNotFound(const std::string& componentName) :
        _message(std::format("Cannot find component ({0})", componentName)) {}

    const char *Registry::ComponentNotFound::what() const noexcept {
        return this->_message.c_str();
    }

    Registry::Registry(uint8_t id) :
        _id(id),
        _maxId(0) {}

    std::size_t Registry::getMaxEntity() const {
        return _maxId;
    }

    std::set<Entity> Registry::getEntities() {
        return _entities;
    }

    void Registry::reset(const Entity& entity) {
        for (auto& component: _components) {
            component.second.erase(entity);
        }
    }
}
