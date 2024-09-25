#include "Entity.hpp"
#include "Registry.hpp"

namespace ecs {
    Entity::Entity(std::size_t id) :
        _id(id) {}

    void Entity::reset() {
        Registry::reset(*this);
    }
}
