#include "EnemyBehavior.hpp"
#include <cmath>

constexpr auto ENEMY_SPEED = 300;
constexpr auto WAVE_AMPLITUDE = 1080;
constexpr auto WAVE_FREQUENCY = 0.01f;
constexpr auto ZIGZAG_SPEED = 300;
constexpr auto DIAGONAL_SPEED = 300;

void EnemyBehavior::classicEnemy(const ecs::Entity& enemy, float timePerTick)
{
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    int16_t speed = -ENEMY_SPEED * timePerTick;
    auto& position = registry.getComponent<ecs::component::Position>(enemy);
    ecs::component::Position const offset( speed, 0);

    position.move(offset);
    if (position.x < 1) {
        registry.removeEntity(enemy);
    }
}

void EnemyBehavior::EnemySinusoidal(const ecs::Entity& enemy, float timePerTick)
{
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto& position = registry.getComponent<ecs::component::Position>(enemy);
    int16_t speedX = -ENEMY_SPEED * timePerTick;
    float waveMotion = WAVE_AMPLITUDE * std::sin(WAVE_FREQUENCY * position.x);
    ecs::component::Position const offset(speedX, waveMotion * timePerTick);
    position.move(offset);

    if (position.x < 1) {
        registry.removeEntity(enemy);
    }
}

void EnemyBehavior::EnemyZigZag(const ecs::Entity& enemy, float timePerTick)
{
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();

    auto& position = registry.getComponent<ecs::component::Position>(enemy);
    int16_t speedX = -ZIGZAG_SPEED * timePerTick;
    int16_t speedY = (std::sin(position.x * 0.03f) > 0 ? DIAGONAL_SPEED : -DIAGONAL_SPEED) * timePerTick;
    ecs::component::Position const offset(speedX, speedY);
    position.move(offset);

    if (position.x < 1 || position.y > position.screenHeight || position.y < 0) {
        registry.removeEntity(enemy);
    }
}