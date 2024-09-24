#include "Registry.hpp"

namespace ECS {
    Entity Registry::createEntity() {
        Entity id = _maxId++;
        return id;
    }

    std::size_t Registry::getMaxEntity() {
        return _maxId;
    }

    std::size_t Registry::_maxId(0);
    std::unordered_map<std::type_index, std::unordered_map<std::size_t, std::any>> ECS::Registry::_components;
}
