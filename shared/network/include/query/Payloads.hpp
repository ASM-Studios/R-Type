#pragma once

#include "Behavior.hpp"
#include "Input.hpp"
#include "Position.hpp"
#include "Tags.hpp"
#include <boost/asio.hpp>
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
        int spriteID;
        int stateID;
        ecs::component::Position position;
        std::array<std::optional<ecs::component::Tag>, 5> tags;
};

struct CreateEnemyEntity : CreateEntity {
        ecs::component::AIModel model;
};

struct UpdateEntity {
        uint64_t id;
        ecs::component::Position position;
};

struct DestroyEntity {
        uint64_t id;
};

struct Connect {
        int id;
};
