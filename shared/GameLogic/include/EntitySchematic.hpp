#pragma once

#include "Registry.hpp"
#include "Position.hpp"
#include "Input.hpp"
#include "Sprite.hpp"
#include "LastShot.hpp"
#include "Animation.hpp"
#include "Behavior.hpp"
#include "Entity.hpp"
#include "Registry.hpp"
#include "GameLogic.hpp"
#include "RegistryManager.hpp"

class EntitySchematic {
    public:
        static ecs::Entity createPlayer(int16_t startX, int16_t startY, int16_t screenWidth, int16_t screenHeight);
        static ecs::Entity createBullet(const ecs::Entity& shooter);
        static ecs::Entity createExplosion(const ecs::Entity& destroyed);
        static ecs::Entity createEnemy(std::size_t id, int16_t x, int16_t y, int spriteID, int stateID, const std::string& model, const std::pair<std::size_t, std::size_t>& screenSize);
};
