#include "EnemyBehavior.hpp"
#include <cmath>

constexpr auto ENEMY_SPEED = 300;
constexpr auto WAVE_AMPLITUDE = 1080;
constexpr auto WAVE_FREQUENCY = 0.01f;
constexpr auto ZIGZAG_SPEED = 300;
constexpr auto DIAGONAL_SPEED = 300;

/**
 * @brief Update the enemy position
 *
 * @param mode The game logic mode
 * @param enemy The enemy entity
 * @param timePerTick The time per tick
 */
void EnemyBehavior::classicEnemy(GameLogicMode mode, const ecs::Entity& enemy, float timePerTick) {
    auto registry = enemy.getRegistry();
    int16_t speed = -ENEMY_SPEED * timePerTick;
    auto& position = registry->getComponent<ecs::component::Position>(enemy);
    ecs::component::Position const offset(speed, 0);

    position.move(offset);
    if (position.x < 1) {
        registry->removeEntity(enemy);
    }
}

/**
 * @brief Update the enemy position
 *
 * @param mode The game logic mode
 * @param enemy The enemy entity
 * @param timePerTick The time per tick
 */
void EnemyBehavior::EnemySinusoidal(GameLogicMode mode, const ecs::Entity& enemy, float timePerTick) {
    auto registry = enemy.getRegistry();
    auto& position = registry->getComponent<ecs::component::Position>(enemy);
    int16_t speedX = -ENEMY_SPEED * timePerTick;
    float waveMotion = WAVE_AMPLITUDE * std::sin(WAVE_FREQUENCY * position.x);
    ecs::component::Position const offset(speedX, waveMotion * timePerTick);
    position.move(offset);

    if (position.x < 1) {
        registry->removeEntity(enemy);
    }
}

/**
 * @brief Update the enemy position
 *
 * @param mode The game logic mode
 * @param enemy The enemy entity
 * @param timePerTick The time per tick
 */
void EnemyBehavior::EnemyZigZag(GameLogicMode mode, const ecs::Entity& enemy, float timePerTick) {
    auto registry = enemy.getRegistry();

    auto& position = registry->getComponent<ecs::component::Position>(enemy);
    int16_t speedX = -ZIGZAG_SPEED * timePerTick;
    int16_t speedY = (std::sin(position.x * 0.03f) > 0 ? DIAGONAL_SPEED : -DIAGONAL_SPEED) * timePerTick;
    ecs::component::Position const offset(speedX, speedY);
    position.move(offset);

    if (position.x < 1 || position.y > position.screenHeight || position.y < 0) {
        registry->removeEntity(enemy);
    }
}
