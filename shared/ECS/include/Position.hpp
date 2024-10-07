#pragma once

#include "Entity.hpp"
#include <cstdint>

namespace ecs::component {
    struct Position {
        uint16_t x;
        uint16_t y;
        void set(ecs::component::Position position);
        void move(ecs::component::Position offset);
    };
}
