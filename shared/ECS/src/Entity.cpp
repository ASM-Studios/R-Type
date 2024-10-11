#include "Entity.hpp"
#include "Registry.hpp"

namespace ecs {
    Entity::Entity(const std::size_t id, EntityType type, Registry& registry) :
        _id(id),
        _type(type),
        _registry(std::ref(registry)) {}

    [[nodiscard]] std::size_t Entity::getID() const {
        return this->_id;
    }

    [[nodiscard]] EntityType Entity::getType() const {
        return _type;
    }

    bool Entity::operator==(const Entity& other) const {
        return this->_id == other._id;
    }

    bool Entity::operator<(const Entity& other) const {
        return _id < other._id;
    }
}
