#pragma once

#include "Entity.hpp"
#include <algorithm>
#include <cstdint>

namespace ecs::component {
    struct Position {
            int16_t x;
            int16_t y;
            std::size_t screenWidth;
            std::size_t screenHeight;
            void set(const Position& position);
            void move(const Position& offset);
            [[nodiscard]] double getDistance(const Position& other) const;
            std::pair<int16_t, int16_t> get() const;
    };
}
