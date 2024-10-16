#pragma once

#include "RegistryManager.hpp"

enum class GameLogicMode;

class EnemyBehavior {
    public:
        EnemyBehavior() = default;
        static void classicEnemy(GameLogicMode mode, const ecs::Entity& enemy, float timePerTick);
        static void EnemySinusoidal(GameLogicMode mode, const ecs::Entity& enemy, float timePerTick);
        static void EnemyZigZag(GameLogicMode, const ecs::Entity& enemy, float timePerTick);
};
