#pragma once

#include "Animation.hpp"
#include "Behavior.hpp"
#include "BehaviorFunc.hpp"
#include "Collision.hpp"
#include "Entity.hpp"
#include "GameLogic.hpp"
#include "Input.hpp"
#include "LastShot.hpp"
#include "Position.hpp"
#include "Registry.hpp"
#include "RegistryManager.hpp"
#include "Sprite.hpp"
#include "Tags.hpp"

constexpr int EXPLOSION_SPRITE_ID = 49;

class EntitySchematic {
    public:
        static ecs::Entity createPlayer(std::shared_ptr<ecs::Registry> registry, int16_t startX, int16_t startY, int16_t screenWidth, int16_t screenHeight);

        static ecs::Entity createTeamPlayer(std::shared_ptr<ecs::Registry> registry, uint64_t id, int16_t x, int16_t y, int spriteID);
        static ecs::Entity createTeamPlayerClient(std::shared_ptr<ecs::Registry> registry, uint64_t id, int16_t x, int16_t y, int spriteID);

        static ecs::Entity createBullet(std::shared_ptr<ecs::Registry> registry);
        static ecs::Entity createBullet(std::shared_ptr<ecs::Registry> registry, const ecs::Entity& shooter);

        static ecs::Entity createExplosion(std::shared_ptr<ecs::Registry> registry, const ecs::Entity& destroyed);
        static ecs::Entity createEnemy(std::shared_ptr<ecs::Registry> registry, std::size_t id, int16_t x, int16_t y, int spriteID, int stateID, const std::string& model, const std::pair<std::size_t, std::size_t>& screenSize);
};
