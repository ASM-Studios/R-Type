#include "BehaviorFunc.hpp"
#include "GameLogic.hpp"
#include "GameLogicMode.hpp"
#include "Logger.hpp"
#include "Sprite.hpp"
#include "Tags.hpp"
#include "TextureLoader.hpp"
#include "query/Payloads.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"
#include "socket/NetworkManager.hpp"
#include <array>
#include <format>
#include <optional>
#include <algorithm>

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

const int16_t JUMP_VELOCITY = -35;
const int16_t GRAVITY = 1;
const int16_t MAX_FALL_SPEED = 20;

void handleSpriteSheetRunner(const ecs::Entity& entity, bool isJumping, int16_t verticalSpeed) {
    auto registry = entity.getRegistry();
    auto& sprite = registry->getComponent<ecs::component::Sprite>(entity);
    const int16_t JUMP_ANIMATION_FRAMES[] = {2, 3, 4};
    int spriteSheetID = 0;

    if (isJumping) {
        spriteSheetID = verticalSpeed < 0 ? JUMP_ANIMATION_FRAMES[0] :
                        (verticalSpeed == 0 ? JUMP_ANIMATION_FRAMES[1] : JUMP_ANIMATION_FRAMES[2]);
    }
    if (sprite.getStateID() != spriteSheetID) {
        sprite.setStateID(spriteSheetID);
    }
}

struct CollisionResult {
    bool isColliding;
    bool fromTop;
};

CollisionResult _checkAABBCollision(const ecs::component::Position& pos1, const std::pair<int, int>& size1,
                                    const ecs::component::Position& pos2, const std::pair<int, int>& size2) {
    CollisionResult result = { false, false };

    int pos1Left = pos1.x - size1.first / 2;
    int pos1Top = pos1.y - size1.second / 2;
    int pos2Left = pos2.x - size2.first / 2;
    int pos2Top = pos2.y - size2.second / 2;

    int pos1Right = pos1Left + size1.first;
    int pos1Bottom = pos1Top + size1.second;
    int pos2Right = pos2Left + size2.first;
    int pos2Bottom = pos2Top + size2.second;

    if (pos1Left < pos2Right &&
        pos1Right > pos2Left &&
        pos1Top < pos2Bottom &&
        pos1Bottom > pos2Top) {
        result.isColliding = true;
        result.fromTop = pos1Bottom <= pos2Top + size2.second / 2;
    }
    
    return result;
}


bool isOnPlatform(const ecs::Entity& runnerEntity, std::shared_ptr<ecs::Registry> registry, int16_t verticalSpeed)
{
    if (verticalSpeed <= 0) return false;

    const auto& runnerPosition = registry->getComponent<ecs::component::Position>(runnerEntity);
    const auto& runnerSize = TextureLoader::getInstance().getSizeFromId(61);

    for (const auto& entity : registry->getEntities()) {
        if (entity == runnerEntity) continue;

        auto tag = registry->getComponent<ecs::component::Tags>(entity);
        if (!tag.hasTag(ecs::component::Tag::Plat)) continue;

        const auto& platformPosition = registry->getComponent<ecs::component::Position>(entity);
        auto& platformSprite = registry->getComponent<ecs::component::Sprite>(entity);

        CollisionResult collision = _checkAABBCollision(
            runnerPosition, runnerSize, platformPosition, 
            TextureLoader::getInstance().getSizeFromId(platformSprite.getSpriteID())
        );

        if (collision.isColliding && collision.fromTop) {
            return true;
        }
    }
    return false;
}

std::optional<ecs::Entity> getPlatformUnderRunner(const ecs::Entity& runnerEntity, std::shared_ptr<ecs::Registry> registry, int16_t verticalSpeed) {
    if (verticalSpeed <= 0) return std::nullopt;

    const auto& runnerPosition = registry->getComponent<ecs::component::Position>(runnerEntity);
    const auto& runnerSize = TextureLoader::getInstance().getSizeFromId(61);

    for (const auto& entity : registry->getEntities()) {
        if (entity == runnerEntity) continue;

        auto tag = registry->getComponent<ecs::component::Tags>(entity);
        if (!tag.hasTag(ecs::component::Tag::Plat)) continue;

        const auto& platformPosition = registry->getComponent<ecs::component::Position>(entity);
        auto& platformSprite = registry->getComponent<ecs::component::Sprite>(entity);
        const auto& platformSize = TextureLoader::getInstance().getSizeFromId(platformSprite.getSpriteID());
        CollisionResult collision = _checkAABBCollision(runnerPosition, runnerSize, platformPosition, platformSize);
        if (collision.isColliding && collision.fromTop) {
            return entity;
        }
    }
    return std::nullopt;
}


void BehaviorFunc::handleRunner(GameLogicMode mode, const ecs::Entity& entity, float timePerTick)
{
    auto registry = entity.getRegistry();
    auto& position = registry->getComponent<ecs::component::Position>(entity);
    auto& input = registry->getComponent<ecs::component::Input>(entity);

    static bool isJumping = true;
    static int16_t verticalSpeed = 0;

    int16_t horizontalSpeed = SPEED * timePerTick / 2;
    bool onGround = position.y >= position.screenHeight - 1;
    ecs::component::Position offset(0, 0);

    if (input.isFlagSet(ecs::component::Input::PressedShoot) && (onGround || isOnPlatform(entity, registry, verticalSpeed))) {
        isJumping = true;
        verticalSpeed = JUMP_VELOCITY;
    }
    if (isJumping) {
        verticalSpeed += GRAVITY;
        if (verticalSpeed > MAX_FALL_SPEED) verticalSpeed = MAX_FALL_SPEED;
        offset.y += verticalSpeed;
        const auto platformEntity = getPlatformUnderRunner(entity, registry, verticalSpeed);
        if (platformEntity && verticalSpeed > 0) {
            const auto& platformPosition = registry->getComponent<ecs::component::Position>(*platformEntity);
            const auto& platformSize = TextureLoader::getInstance().getSizeFromId(
                registry->getComponent<ecs::component::Sprite>(*platformEntity).getSpriteID()
            );

            isJumping = false;
            verticalSpeed = 0;
            offset.y = 0;
            position.y = platformPosition.y - TextureLoader::getInstance().getSizeFromId(61).second - 20;

            return;
        }
    }
    if (onGround && !isJumping) {
        verticalSpeed = 0;
        offset.y = 0;
    }
    else if (!isOnPlatform(entity, registry, verticalSpeed) && !isJumping) {
        isJumping = true;
    }
    position.move(offset);

    if (position.x <= 1 || position.x >= position.screenWidth - 1) {
        Logger::log(LogLevel::INFO, "Runner DIED");
        registry->removeEntity(entity);
        return;
    }
}




void BehaviorFunc::handlePlatform(GameLogicMode mode, const ecs::Entity& entity, const float timePerTick) {
    auto registry = entity.getRegistry();
    auto& position = registry->getComponent<ecs::component::Position>(entity);
    ecs::component::Position offset(0, 0);

    offset.x = -SPEED / 2 * timePerTick;
    position.move(offset);

    if (position.x - 10 < 0) {
        registry->removeEntity(entity);
    }
}
