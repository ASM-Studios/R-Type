#include "BehaviorFunc.hpp"

constexpr auto SPEED = 400;

void BehaviorFunc::updateBullet(const ecs::Entity &bullet, float timePerTick)
{
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    int16_t speed = SPEED * 2 * timePerTick;
    auto& position = registry.getComponent<ecs::component::Position>(bullet);
    ecs::component::Position const offset( speed, 0);

    position.move(offset);
    if (position.x >= position.screenWidth - 1) {
        registry.removeEntity(bullet);
    }
}
void BehaviorFunc::handleInput(const ecs::Entity& entity, float timePerTick)
{
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto& input = registry.getComponent<ecs::component::Input>(entity);
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

    auto currentTime = std::chrono::steady_clock::now();
    float const deltaTimeShot = std::chrono::duration<float>(currentTime - lastShot.lastShotTime).count();

    if (input.isFlagSet(ecs::component::Input::ReleaseShoot) && deltaTimeShot >= SHOOT_COOLDOWN
        && registry.contains<ecs::component::LastShot>(entity) ) {
        EntitySchematic::createBullet(entity);
        lastShot.lastShotTime = currentTime;
    }
    /*input.clearFlag(ecs::component::Input::MoveLeft | ecs::component::Input::MoveRight |
                    ecs::component::Input::MoveUp | ecs::component::Input::MoveDown | ecs::component::Input::ReleaseShoot);*/
}
