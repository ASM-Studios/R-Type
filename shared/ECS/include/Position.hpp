#pragma once

#include "Entity.hpp"
#include <cstdint>

namespace ecs::component {
    struct Position {
        uint16_t x;
        uint16_t y;
    };
}

namespace ecs::system {
    struct Position {
        static void set(Entity entity, ecs::component::Position position);
        static void move(Entity entity, ecs::component::Position offset);
    };
}
