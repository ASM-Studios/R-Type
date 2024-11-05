#include "BehaviorFunc.hpp"
#include "GameLogic.hpp"
#include "GameLogicMode.hpp"
#include "Sprite.hpp"
#include "Tags.hpp"
#include "query/Payloads.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"
#include "socket/NetworkManager.hpp"
#include <array>
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
const int16_t GRAVITY = 2;
const int16_t MAX_FALL_SPEED = 20;


/**
 * @brief Update the runner sprite sheet for jumping animation
 */
void handleSpriteSheetRunner(const ecs::Entity& entity, bool isJumping, int16_t verticalSpeed) {
    auto registry = entity.getRegistry();
    auto& sprite = registry->getComponent<ecs::component::Sprite>(entity);
    auto& position = registry->getComponent<ecs::component::Position>(entity);
    const int16_t JUMP_ANIMATION_FRAMES[] = {2, 3, 4};
    int spriteSheetID = 0;

    if (isJumping) {
        if (verticalSpeed < 0) {
            spriteSheetID = JUMP_ANIMATION_FRAMES[0];
        } else if (verticalSpeed == 0) {
            spriteSheetID = JUMP_ANIMATION_FRAMES[1];
        } else {
            spriteSheetID = JUMP_ANIMATION_FRAMES[2];
        }
    } else {
        spriteSheetID = 0;
    }

    if (sprite.getStateID() != spriteSheetID) {
        sprite.setStateID(spriteSheetID);
    }
}

bool isOnPlatform(const ecs::Entity& runner, const std::shared_ptr<ecs::Registry>& registry, int16_t verticalSpeed)
{
    auto& runPos = registry->getComponent<ecs::component::Position>(runner);
    auto &runSprite = registry->getComponent<ecs::component::Sprite>(runner);
    std::pair<int, int> runSize = TextureLoader::getInstance().getTexture(runSprite.getSpriteID()).getSize();
    for (const ecs::Entity &entity : registry->getEntities()) {
        auto tag = registry->getComponent<ecs::component::Tags>(entity);
        if (tag.hasTag(ecs::component::Tag::Plat)) {
            auto &platformPosition = registry->getComponent<ecs::component::Position>(entity);
            auto &platformSprite = registry->getComponent<ecs::component::Sprite>(runner);
            std::pair<int, int> platSize = TextureLoader::getInstance().getTexture(platformSprite.getSpriteID()).getSize();
            bool isAbovePlatform = runPos.y + runSize.second <= platformPosition.y &&
                                runPos.y + runSize.second + verticalSpeed >= platformPosition.y;
            bool xOverlap = runPos.x < platformPosition.x + platSize.first &&
                            runPos.x + runSize.first > platformPosition.x;

            if (isAbovePlatform && xOverlap) {
                return true;
            }
        }
        
    }
    return false;
}



void BehaviorFunc::handleRunner(GameLogicMode mode, const ecs::Entity& entity, float timePerTick) {
    auto registry = entity.getRegistry();
    auto& position = registry->getComponent<ecs::component::Position>(entity);
    auto& input = registry->getComponent<ecs::component::Input>(entity);
    static bool isJumping = true;
    static int16_t verticalSpeed = 0;

    int16_t horizontalSpeed = SPEED * timePerTick / 2;

    bool onGround = position.y >= position.screenHeight - 1;

    ecs::component::Position offset(0, 0);

    if (input.isFlagSet(ecs::component::Input::MoveLeft)) {
        offset.x -= horizontalSpeed;
    }
    if (input.isFlagSet(ecs::component::Input::MoveRight)) {
        offset.x += horizontalSpeed;
    }

    if (input.isFlagSet(ecs::component::Input::PressedShoot) && (onGround || isOnPlatform(entity, registry, verticalSpeed))) {
        isJumping = true;
        verticalSpeed = JUMP_VELOCITY;
    }

    if (isJumping) {
        verticalSpeed += GRAVITY;
        if (verticalSpeed > MAX_FALL_SPEED) {
            verticalSpeed = MAX_FALL_SPEED;
        }
        offset.y += verticalSpeed;
        if (isOnPlatform(entity, registry, verticalSpeed)) {
            isJumping = false;
            verticalSpeed = 0;
            offset.y = 0;
        }
    }

    position.move(offset);
    if (position.y >= position.screenHeight - 1) {
        position.y = position.screenHeight - 1;
        isJumping = false;
        verticalSpeed = 0;
    }
    if (position.x <= 1 || position.x >= position.screenWidth - 1) {
        Logger::log(LogLevel::INFO, "Runner DIED - hit horizontal boundary");
        registry->removeEntity(entity);
    }
    //Logger::log(LogLevel::WARNING, std::format("Runner position: x={}, y={}, jumping={}", position.x, position.y, isJumping));
    handleSpriteSheetRunner(entity, isJumping, verticalSpeed);
}

void BehaviorFunc::handlePlatform(GameLogicMode mode, const ecs::Entity& entity, const float timePerTick)
{
    auto registry = entity.getRegistry();
    auto& position = registry->getComponent<ecs::component::Position>(entity);
    ecs::component::Position offset(0, 0);
    
    offset.x = -SPEED / 2 * timePerTick;
    position.move(offset);
    if (position.x - 10 < 0) {
        registry->removeEntity(entity);
    }
  
}
