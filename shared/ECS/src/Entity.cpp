#include "Entity.hpp"
#include "Registry.hpp"

namespace ecs {
    Entity::Entity(uint64_t id, std::shared_ptr<Registry> registry) :
        _id(id),
        _registry(registry) {}

    [[nodiscard]] uint64_t Entity::getID() const {
        return this->_id;
    }

    bool Entity::operator==(const Entity& other) const {
        return this->_id == other._id;
    }

    bool Entity::operator<(const Entity& other) const {
        return _id < other._id;
    }

    std::shared_ptr<Registry> Entity::getRegistry() const {
        return this->_registry;
    }
}
