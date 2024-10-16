#include "BehaviorFunc.hpp"
#include "GameLogic.hpp"
#include "GameLogicMode.hpp"
#include "RegistryManager.hpp"
#include "Tags.hpp"
#include "query/Payloads.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"
#include "socket/ServerManager.hpp"
#include <array>
#include <optional>

constexpr auto SPEED = 400;

void BehaviorFunc::updateBullet(GameLogicMode mode, const ecs::Entity& bullet, float timePerTick) {
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    int16_t speed = SPEED * 10 * timePerTick;
    auto& position = registry.getComponent<ecs::component::Position>(bullet);
    ecs::component::Position const offset(speed, 0);

    position.move(offset);
    if (position.x >= position.screenWidth - 1) {
        registry.removeEntity(bullet);
    }
}

void BehaviorFunc::setSpriteSheetFromInput(GameLogicMode mode, const ecs::Entity& entity, float deltaTime) {
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto& input = registry.getComponent<ecs::component::Input>(entity);
    auto& sprite = registry.getComponent<ecs::component::Sprite>(entity);
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

static void sendBullet(const ecs::Entity& entity) {
    for (const auto& [destEntity, destClient]: ecs::RegistryManager::getInstance().getRegistry().getEntities<network::Client>()) {
        const ecs::Entity& bullet = EntitySchematic::createBullet(entity);

        auto position = ecs::RegistryManager::getInstance().getRegistry().getComponent<ecs::component::Position>(bullet);
        auto tags = static_cast<std::array<std::optional<ecs::component::Tag>, 5>>(ecs::RegistryManager::getInstance().getRegistry().getComponent<ecs::component::Tags>(bullet));

        CreateEntity payload{entity.getID(), 13, position, tags};
        TypedQuery tq(RequestType::CREATE_ENTITY, payload);
        network::socket::udp::ServerManager::getInstance().getServer().send(destClient.getIP().to_string(), destClient.getPort(), RawRequest(tq));
    }
}

void BehaviorFunc::handleInput(GameLogicMode mode, const ecs::Entity& entity, float timePerTick) {
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto& input = registry.getComponent<ecs::component::Input>(entity);
    if (input.inputFlags == 0) {
        setSpriteSheetFromInput(mode, entity, timePerTick);
        return;
    }

    auto& position = registry.getComponent<ecs::component::Position>(entity);
    auto& lastShot = registry.getComponent<ecs::component::LastShot>(entity);
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

    if (input.isFlagSet(ecs::component::Input::ReleaseShoot) && deltaTimeShot >= SHOOT_COOLDOWN && registry.contains<ecs::component::LastShot>(entity)) {
        if (isPureServer(mode)) {
            sendBullet(entity);
        }
        lastShot.lastShotTime = currentTime;
    }
}
