#pragma once

#include "Input.hpp"
#include "Position.hpp"
#include "Tags.hpp"
#include <cstdint>
#include <optional>

struct UpdatePlayer {
        ecs::component::Position position;
};

struct UpdateTeamPlayer {
        uint64_t id;
        ecs::component::Input input;
        ecs::component::Position position;
};

struct CreateEntity {
        uint64_t id;
        uint8_t spriteID;
        ecs::component::Position position;
        std::array<std::optional<ecs::component::Tag>, 5> tags;
};

struct UpdateEntity {
        uint64_t id;
        ecs::component::Position position;
};
