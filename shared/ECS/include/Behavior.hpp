#pragma once
#include "Entity.hpp"
#include "Logger.hpp"
#include "EnemyBehavior.hpp"

namespace ecs::component {
    struct Behavior {
        std::function<void (const ecs::Entity entity, float timePerTick)>func;
        static Behavior stringToBehaviorModel(const std::string& modelStr);
    };
}