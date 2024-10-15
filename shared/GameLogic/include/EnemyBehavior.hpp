#pragma once
#include "RegistryManager.hpp"
#include "EntitySchematic.hpp"

class EnemyBehavior {
    public:
        EnemyBehavior() = default;
        static void classicEnemy(const ecs::Entity &enemy, float timePerTick);
        static void EnemySinusoidal(const ecs::Entity& enemy, float timePerTick);
        static void EnemyZigZag(const ecs::Entity& enemy, float timePerTick);
};