#include "EntitySchematic.hpp"

ecs::Entity EntitySchematic::createPlayer(int16_t startX, int16_t startY, int16_t screenWidth, int16_t screenHeight) {
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto player = registry.createEntity<>(0);
    ecs::component::Position const position = {startX, startY, static_cast<size_t>(screenWidth), static_cast<size_t>(screenHeight)};

    registry.setComponent<ecs::component::Tags>(player, ecs::component::Tags({ecs::component::Tag::Ally}));
    registry.setComponent<ecs::component::Position>(player, position);
    registry.setComponent<ecs::component::Input>(player, {});
    registry.setComponent<ecs::component::Sprite>(player, {22, 2});
    registry.setComponent<ecs::component::LastShot>(player, {});
    registry.setComponent<ecs::component::Behavior>(player, {&BehaviorFunc::handleInput});
    return player;
}

ecs::Entity EntitySchematic::createBullet(const ecs::Entity& shooter)
{
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto& shooterPosition = registry.getComponent<ecs::component::Position>(shooter);
    auto bullet = registry.createEntity<>();

    registry.setComponent<ecs::component::Tags>(bullet, ecs::component::Tags({ecs::component::Tag::Bullet, ecs::component::Tag::Ally}));
    registry.setComponent<ecs::component::Position>(bullet, {static_cast<int16_t>(shooterPosition.x + 50), shooterPosition.y, shooterPosition.screenWidth, shooterPosition.screenHeight});
    registry.setComponent<ecs::component::Sprite>(bullet, {13, 0});
    registry.setComponent<ecs::component::Animation>(bullet, {.frameTime = 0.1F});
    registry.setComponent<ecs::component::Behavior>(bullet, {&BehaviorFunc::updateBullet});
    registry.setComponent<ecs::component::Collision>(bullet, {ecs::component::CollisionAlgorithm::AABB});
    return bullet;
}

ecs::Entity EntitySchematic::createExplosion(const ecs::Entity& destroyed)
{
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto& destroyedPosition = registry.getComponent<ecs::component::Position>(destroyed);
    auto explosion = registry.createEntity<>();

    registry.setComponent<ecs::component::Tags>(explosion, ecs::component::Tags({ecs::component::Tag::Explosion}));
    registry.setComponent<ecs::component::Position>(explosion, {static_cast<int16_t>(destroyedPosition.x + 50), destroyedPosition.y, destroyedPosition.screenWidth, destroyedPosition.screenHeight});
    registry.setComponent<ecs::component::Sprite>(explosion, {29, 0});
    registry.setComponent<ecs::component::Animation>(explosion, {.frameTime = 0.1F});

    return explosion;
}

ecs::Entity EntitySchematic::createEnemy(std::size_t id, int16_t x, int16_t y, int spriteID, int stateID, const std::string& model, const std::pair<std::size_t, std::size_t>& screenSize)
{
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto enemy = registry.createEntity<>();

    registry.setComponent<ecs::component::Tags>(enemy, ecs::component::Tags({ecs::component::Tag::Enemy}));
    registry.setComponent<ecs::component::Position>(enemy, {x, y, static_cast<size_t>(static_cast<int16_t>(screenSize.first)), static_cast<size_t>(static_cast<int16_t>(screenSize.second))});
    registry.setComponent<ecs::component::Sprite>(enemy, {spriteID, stateID});
    registry.setComponent<ecs::component::AI>(enemy, {ecs::component::AI::stringToAIModel(model)});
    registry.setComponent<ecs::component::Animation>(enemy, {.frameTime = 0.1F});
    return enemy;
}
