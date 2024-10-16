#pragma once
#include "EnemyBehavior.hpp"
#include "Entity.hpp"
#include "Logger.hpp"

namespace ecs::component {
    struct Behavior {
            std::function<void(GameLogicMode mode, const ecs::Entity entity, float timePerTick)> func;
            static Behavior stringToBehaviorModel(const std::string& modelStr);
    };
}
