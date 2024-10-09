#include "Collison.hpp"
#include "Logger.hpp"
#include <algorithm>


Collision::Collision() = default;

void Collision::updateCollisions()
{
    auto sortedEntities = sortedEntitiesVec();
    checkCollisions(sortedEntities);
}

std::vector<std::pair<int16_t, ecs::Entity>> Collision::sortedEntitiesVec() {
    std::vector<std::pair<int16_t, ecs::Entity>> sortedEntities;

    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    for (const auto& entity : registry.getEntities()) {
        if ((entity.getType() == ecs::EntityType::Bullet || entity.getType() == ecs::EntityType::Enemy) &&
            registry.contains<ecs::component::Position>(entity)) {
            auto& position = registry.getComponent<ecs::component::Position>(entity);
            sortedEntities.emplace_back(position.x, entity);
        }
    }
    std::sort(sortedEntities.begin(), sortedEntities.end(),
    [](const std::pair<int16_t, ecs::Entity>& a, const std::pair<int16_t, ecs::Entity>& b) {
      return a.first < b.first;
    });

    return sortedEntities;
}

void Collision::checkCollisions(const std::vector<std::pair<int16_t, ecs::Entity>>& sortedEntities)
{
    for (size_t i = 0; i < sortedEntities.size(); ++i) {
        const auto& [x1, bullet] = sortedEntities[i];
        if (bullet.getType() != ecs::EntityType::Bullet)
            continue;
        for (size_t j = i + 1; j < sortedEntities.size(); ++j) {
            const auto& [x2, enemy] = sortedEntities[j];

            short const MAX_BULLET_WIDTH = 50;
            if (x2 > x1 + MAX_BULLET_WIDTH)
                break;

            ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
            if (enemy.getType() == ecs::EntityType::Enemy &&
                registry.contains<ecs::component::Position>(enemy)
                && registry.contains<ecs::component::Sprite>(enemy)) {
                if (intersects(bullet, enemy)) {
                    EntitySchematic::createExplosion(enemy);
                    registry.removeEntity(bullet);
                    registry.removeEntity(enemy);
                    break;
                }
            }
        }
    }
}

bool Collision::intersects(const ecs::Entity& entity1, const ecs::Entity& entity2)
{
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();

    auto& bulletPosition = registry.getComponent<ecs::component::Position>(entity1);
    auto& enemyPosition = registry.getComponent<ecs::component::Position>(entity2);
    auto& bulletSprite = registry.getComponent<ecs::component::Sprite>(entity1);
    auto& enemySprite = registry.getComponent<ecs::component::Sprite>(entity2);
    auto& textureLoader = TextureLoader::getInstance();
    std::pair<int, int> const bulletSize = textureLoader.getSizeFromId(bulletSprite.getSpriteID());
    std::pair<int, int> const enemySize = textureLoader.getSizeFromId(enemySprite.getSpriteID());
    bool const xOverlap = (bulletPosition.x < enemyPosition.x + enemySize.first) && (bulletPosition.x + bulletSize.first > enemyPosition.x);
    bool const yOverlap = (bulletPosition.y < enemyPosition.y + enemySize.second) && (bulletPosition.y + bulletSize.second > enemyPosition.y);
    return xOverlap && yOverlap;
}

