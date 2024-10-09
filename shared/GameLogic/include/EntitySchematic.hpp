#pragma once

#include "Registry.hpp"
#include "Position.hpp"
#include "Input.hpp"
#include "Sprite.hpp"
#include "LastShot.hpp"
#include "RegistryManager.hpp"

class EntitySchematic {
    public:
        static ecs::Entity createPlayer(int16_t startX, int16_t startY, int16_t screenWidth, int16_t screenHeight);
        static ecs::Entity createBullet(const ecs::Entity& shooter);
};
