#include "Entity.hpp"
#include "Registry.hpp"

namespace ecs {
    Entity::Entity(uint64_t id, Registry& registry) :
        _id(id),
        _registry(std::ref(registry)) {}

    [[nodiscard]] uint64_t Entity::getID() const {
        return this->_id;
    }

    bool Entity::operator==(const Entity& other) const {
        return this->_id == other._id;
    }

    bool Entity::operator<(const Entity& other) const {
        return _id < other._id;
    }
}
