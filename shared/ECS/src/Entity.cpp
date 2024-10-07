#include "Entity.hpp"
#include "Registry.hpp"

namespace ecs {
    Entity::Entity(const std::size_t id) :
        _id(id) {}

    [[nodiscard]] std::size_t Entity::getID() const {
        return this->_id;
    }

    bool Entity::operator==(const Entity& other) const {
        return this->_id == other._id;
    }

    bool Entity::operator<(const Entity& other) const {
        return _id < other._id;
    }
}
