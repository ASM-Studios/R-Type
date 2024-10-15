#include "Position.hpp"
#include <cmath>

namespace ecs::component {
    void Position::set(const Position& position) {
        this->x = std::clamp(position.x, static_cast<int16_t>(0), static_cast<int16_t>(screenWidth - 1));
        this->y = std::clamp(position.y, static_cast<int16_t>(0), static_cast<int16_t>(screenHeight - 1));
    }

    void Position::move(const Position& offset) {
        this->x = std::clamp(static_cast<int16_t>(this->x + offset.x), static_cast<int16_t>(0), static_cast<int16_t>(screenWidth - 1));
        this->y = std::clamp(static_cast<int16_t>(this->y + offset.y), static_cast<int16_t>(0), static_cast<int16_t>(screenHeight - 1));
    }

    double Position::getDistance(const Position& other) const {
        return std::sqrt(std::pow(this->x - other.x, 2) + std::pow(this->y - other.y, 2));
    }

    std::pair<int16_t, int16_t> Position::get() const {
        return std::make_pair(this->x, this->y);
    }
}
