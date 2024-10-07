#include "Position.hpp"
#include "Registry.hpp"

namespace ecs::component {
    void Position::set(ecs::component::Position position) {
        this->x = position.x;
        this->y = position.y;
    }
    
    void Position::move(ecs::component::Position offset) {
        this->x += offset.x;
        this->y += offset.y;
    }
}
