#pragma once

#include "Entity.hpp"
#include <cstdint>
#include <algorithm>

namespace ecs::component {
    struct Position {
        int16_t x;
        int16_t y;
        std::size_t screenWidth;
        std::size_t screenHeight;
        void set(const Position& position);
        void move(const Position& offset);
        std::pair<int16_t, int16_t> get() const;
    };
}
