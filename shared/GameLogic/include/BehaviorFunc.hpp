#pragma once

#include "RegistryManager.hpp"

enum class GameLogicMode;

class BehaviorFunc {
    public:
        BehaviorFunc() = default;
        static void updateBullet(GameLogicMode mode, const ecs::Entity& bullet, float timePerTick);
        static void handleInput(GameLogicMode, const ecs::Entity& bullet, float timePerTick);
        static void setSpriteSheetFromInput(GameLogicMode, const ecs::Entity& entity, float timePerTick);
        static void handleRunner(GameLogicMode, const ecs::Entity& entity, float timePerTick);
        static void handlePlatform(GameLogicMode mode, const ecs::Entity&entity, float timePerTick);
};
