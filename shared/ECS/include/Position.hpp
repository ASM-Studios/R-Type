#pragma once

#include "Entity.hpp"
#include <cstdint>

namespace ecs::component {
    struct Position {
        uint16_t x;
        uint16_t y;
        void set(Position position);
        void move(Position offset);
    };
}
