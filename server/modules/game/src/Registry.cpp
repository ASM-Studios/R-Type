#include "Registry.hpp"
#include <format>
#include <set>

namespace ecs {
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

    std::size_t Registry::_maxId(0);
    std::set<Entity> ecs::Registry::_entities;
    std::unordered_map<std::type_index, std::unordered_map<Entity, std::any>> ecs::Registry::_components;
}
