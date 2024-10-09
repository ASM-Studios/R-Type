#pragma once

#include "Entity.hpp"
#include <cstdint>

namespace ecs::component {
    struct Position {
        int16_t x;
        int16_t y;
        void set(ecs::component::Position position);
        void move(ecs::component::Position offset);
    };
}
