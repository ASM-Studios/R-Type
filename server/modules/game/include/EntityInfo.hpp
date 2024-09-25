#pragma once

#include "Entity.hpp"
#include <cstdint>

namespace ecs::component {
    struct PlayerComponent {
    };

    struct Position {
        uint16_t x;
        uint16_t y;
    };

    struct Velocity {
        int32_t x;
        int32_t y;
    };
}

namespace ecs::system {
    struct PlayerSystem {
        static std::vector<ecs::Entity> getPlayers();
    };
}
