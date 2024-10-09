#include "GameLogic.hpp"
#include <algorithm>

void GameLogic::update() {
    for (const auto& entity : _registry.getEntities()) {
        if (entity.getType() == ecs::EntityType::Player
            && _registry.contains<ecs::component::Input>(entity)
            && _registry.contains<ecs::component::Position>(entity) ) {
            handleInput(entity);
        }
    }
    updateBullets();
}


void GameLogic::handleInput(const ecs::Entity& entity) {
    auto& input = _registry.getComponent<ecs::component::Input>(entity);
    auto& position = _registry.getComponent<ecs::component::Position>(entity);

    ecs::component::Position offset(0, 0);
    if (input.isFlagSet(ecs::component::Input::MoveLeft)) {
        offset.x -= SPEED;
    }
    if (input.isFlagSet(ecs::component::Input::MoveRight)) {
        offset.x += SPEED;
    }
    if (input.isFlagSet(ecs::component::Input::MoveUp)) {
        offset.y -= SPEED;
    }
    if (input.isFlagSet(ecs::component::Input::MoveDown)) {
        offset.y += SPEED;
    }
    position.move(offset);

    if (input.isFlagSet(ecs::component::Input::ReleaseShoot)) {
        createBullet(entity);
        input.clearFlag(ecs::component::Input::PressedShoot);
    }
}

void GameLogic::createBullet(const ecs::Entity& shooter) {
    auto& shooterPosition = _registry.getComponent<ecs::component::Position>(shooter);
    auto bullet = _registry.createEntity<ecs::component::Input, ecs::component::Position>(ecs::EntityType::Bullet);
    ecs::RegistryManager::getInstance().getRegistry().setComponent<ecs::component::Position>(bullet, {static_cast<int16_t>(shooterPosition.x + 50), shooterPosition.y, shooterPosition.screenWidth, shooterPosition.screenHeight});
    ecs::RegistryManager::getInstance().getRegistry().setComponent<ecs::component::Sprite>(bullet, {13, 0});
}



void GameLogic::updateBullets() {
    for (const auto& bullet : _registry.getEntities()) {
        if (bullet.getType() == ecs::EntityType::Bullet
            && _registry.contains<ecs::component::Position>(bullet)
            && _registry.contains<ecs::component::Sprite>(bullet)) {
            auto& position = _registry.getComponent<ecs::component::Position>(bullet);
            ecs::component::Position const offset(SPEED, 0);

            position.move(offset);
            if (position.x >= position.screenWidth - 1)
                _registry.removeEntity(bullet);
        }
    }
}



