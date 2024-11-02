#include "BehaviorFunc.hpp"
#include "GameLogic.hpp"
#include "GameLogicMode.hpp"
#include "RegistryManager.hpp"
#include "Tags.hpp"
#include "query/Payloads.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"
#include "socket/NetworkManager.hpp"
#include <array>
#include <optional>

constexpr auto SPEED = 400;

/**
 * @brief Update the bullet position
 *
 * @param mode The game logic mode
 * @param bullet The bullet entity
 * @param timePerTick The time per tick
 */
void BehaviorFunc::updateBullet(GameLogicMode mode, const ecs::Entity& bullet, float timePerTick) {
    auto registry = bullet.getRegistry();
    int16_t speed = SPEED * 10 * timePerTick;
    auto& position = registry->getComponent<ecs::component::Position>(bullet);
    ecs::component::Position const offset(speed, 0);

    position.move(offset);
    if (position.x >= position.screenWidth - 1) {
        registry->removeEntity(bullet);
    }
}

/**
 * @brief Update the player sprite sheet from input
 *
 * @param mode The game logic mode
 * @param player The player entity
 * @param timePerTick The time per tick
 */
void BehaviorFunc::setSpriteSheetFromInput(GameLogicMode mode, const ecs::Entity& entity, float deltaTime) {
    auto registry = entity.getRegistry();
    auto& input = registry->getComponent<ecs::component::Input>(entity);
    auto& sprite = registry->getComponent<ecs::component::Sprite>(entity);
    static float upDuration = 0.0F;
    static float downDuration = 0.0F;
    int spriteSheetID = 2;

    if (input.isFlagSet(ecs::component::Input::MoveUp) && input.isFlagSet(ecs::component::Input::MoveDown)) {
        upDuration = std::max(0.0F, upDuration - deltaTime);
        downDuration = std::max(0.0F, downDuration - deltaTime);
    } else if (input.isFlagSet(ecs::component::Input::MoveUp)) {
        upDuration += deltaTime;
        downDuration = 0.0F;
    } else if (input.isFlagSet(ecs::component::Input::MoveDown)) {
        downDuration += deltaTime;
        upDuration = 0.0F;
    } else {
        upDuration = 0;
        downDuration = 0;
    }

    if (upDuration > 0.33F) {
        spriteSheetID = 4;
    } else if (upDuration > 0.17F) {
        spriteSheetID = 3;
    } else if (upDuration > 0.0F) {
        spriteSheetID = 2;
    }

    if (downDuration > 0.33F) {
        spriteSheetID = 0;
    } else if (downDuration > 0.17F) {
        spriteSheetID = 1;
    } else if (downDuration > 0.0F) {
        spriteSheetID = 2;
    }
    if (sprite.getStateID() != spriteSheetID) {
        sprite.setStateID(spriteSheetID);
    }
}

/**
 * @brief Send a bullet to all clients
 *
 * @param entity The entity that shot the bullet
 */
static void sendBullet(const ecs::Entity& entity) {
    auto registry = entity.getRegistry();
    for (const auto& [destEntity, destClient]: registry->getEntities<std::shared_ptr<network::Client>>()) {
        const ecs::Entity& bullet = EntitySchematic::createBullet(entity.getRegistry(), entity);

        auto position = registry->getComponent<ecs::component::Position>(bullet);
        auto tags = static_cast<std::array<std::optional<ecs::component::Tag>, 5>>(registry->getComponent<ecs::component::Tags>(bullet));

        CreateEntity payload{entity.getID(), 13, 0, position, tags};
        TypedQuery tq(RequestType::CREATE_ENTITY, payload);
        network::socket::NetworkManager::getInstance().send(destClient, RawRequest(tq), network::socket::Mode::UDP);
    }
}

/**
 * @brief Handle the input of the player
 *
 * @param mode The game logic mode
 * @param entity The player entity
 * @param timePerTick The time per tick
 */
void BehaviorFunc::handleInput(GameLogicMode mode, const ecs::Entity& entity, float timePerTick) {
    auto registry = entity.getRegistry();
    auto& input = registry->getComponent<ecs::component::Input>(entity);
    if (input.inputFlags == 0) {
        setSpriteSheetFromInput(mode, entity, timePerTick);
        return;
    }

    auto& position = registry->getComponent<ecs::component::Position>(entity);
    auto& lastShot = registry->getComponent<ecs::component::LastShot>(entity);
    int16_t speed = SPEED * timePerTick;

    ecs::component::Position offset(0, 0);
    if (input.isFlagSet(ecs::component::Input::MoveLeft)) {
        offset.x -= speed;
    }
    if (input.isFlagSet(ecs::component::Input::MoveRight)) {
        offset.x += speed;
    }
    if (input.isFlagSet(ecs::component::Input::MoveUp)) {
        offset.y -= speed;
    }
    if (input.isFlagSet(ecs::component::Input::MoveDown)) {
        offset.y += speed;
    }
    position.move(offset);
    setSpriteSheetFromInput(mode, entity, timePerTick);
    auto currentTime = std::chrono::steady_clock::now();
    float const deltaTimeShot = std::chrono::duration<float>(currentTime - lastShot.lastShotTime).count();

    if (input.isFlagSet(ecs::component::Input::ReleaseShoot) && deltaTimeShot >= SHOOT_COOLDOWN && registry->contains<ecs::component::LastShot>(entity)) {
        if (isPureServer(mode)) {
            sendBullet(entity);
        }
        lastShot.lastShotTime = currentTime;
    }
}
