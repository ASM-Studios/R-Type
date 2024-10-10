#pragma once
#include "RegistryManager.hpp"
#include "EntitySchematic.hpp"

class BehaviorFunc {
    public:
        BehaviorFunc() = default;
        static void updateBullet(const ecs::Entity &bullet, float timePerTick);
        static void handleInput(const ecs::Entity &bullet, float timePerTick);
};