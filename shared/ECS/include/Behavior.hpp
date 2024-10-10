#pragma once
#include "Entity.hpp"

namespace ecs::component {
    struct Behavior {
        std::function<void (const ecs::Entity entity, float timePerTick)>func;
    };
}