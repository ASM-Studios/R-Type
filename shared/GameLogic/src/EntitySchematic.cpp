#include "EntitySchematic.hpp"
#include "Entity.hpp"
#include "Registry.hpp"


ecs::Entity EntitySchematic::createPlayer(int16_t startX, int16_t startY, int16_t screenWidth, int16_t screenHeight) {
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto player = registry.createEntity<ecs::component::Position, ecs::component::Input, ecs::component::Sprite, ecs::component::LastShot>(0,ecs::EntityType::Player);
    ecs::component::Position position = {startX, startY, static_cast<size_t>(screenWidth), static_cast<size_t>(screenHeight)};
    registry.setComponent<ecs::component::Position>(player, position);
    registry.setComponent<ecs::component::Input>(player, {});
    registry.setComponent<ecs::component::Sprite>(player, {22, 0});
    registry.setComponent<ecs::component::LastShot>(player, {});

    return player;
}

ecs::Entity EntitySchematic::createBullet(const ecs::Entity& shooter)
{
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto& shooterPosition = registry.getComponent<ecs::component::Position>(shooter);
    auto bullet = registry.createEntity<ecs::component::Position, ecs::component::Sprite>(ecs::EntityType::Bullet);

    registry.setComponent<ecs::component::Position>(bullet, {static_cast<int16_t>(shooterPosition.x + 50), shooterPosition.y, shooterPosition.screenWidth, shooterPosition.screenHeight});
    registry.setComponent<ecs::component::Sprite>(bullet, {13, 0});

    return bullet;
}
