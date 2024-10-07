#include "Entity.hpp"
#include "Registry.hpp"
#include <format>
#include <set>

namespace ecs {
    Registry::ComponentNotFound::ComponentNotFound(const std::string& componentName) :
        _message(std::format("Cannot find component ({0})", componentName)) {}

    const char *Registry::ComponentNotFound::what() const noexcept {
        return this->_message.c_str();
    }

    std::size_t Registry::getMaxEntity() {
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
