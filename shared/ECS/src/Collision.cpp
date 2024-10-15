#include "Collision.hpp"
#include "Client.hpp"
#include "Tags.hpp"

bool checkAABBCollision(const ecs::component::Position& pos1, const std::pair<int, int>& size1,
                        const ecs::component::Position& pos2, const std::pair<int, int>& size2) {
    return (pos1.x < pos2.x + size2.first &&
            pos1.x + size1.first > pos2.x &&
            pos1.y < pos2.y + size2.second &&
            pos1.y + size1.second > pos2.y);
}

bool checkCircleCollision(const ecs::component::Position& pos1, const std::pair<int, int>& size1,
                          const ecs::component::Position& pos2, const std::pair<int, int>& size2) {
    float dx = pos1.x + size1.first / 2 - (pos2.x + size2.first / 2);
    float dy = pos1.y + size1.second / 2 - (pos2.y + size2.second / 2);
    float distance = sqrt(dx * dx + dy * dy);

    float radius1 = std::min(size1.first, size1.second) / 2;
    float radius2 = std::min(size2.first, size2.second) / 2;

    return distance < (radius1 + radius2);
}

void ecs::component::Collision::checkCollision(const ecs::Entity& self) {
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    if (!registry.contains<Position>(self) || !registry.contains<Sprite>(self) || !registry.contains<Tags>(self)) {
        return;
    }

    auto& selfPosition = registry.getComponent<Position>(self);
    auto& selfSprite = registry.getComponent<Sprite>(self);
    ecs::component::Tags tags = registry.getComponent<Tags>(self);
    std::pair<int, int> selfSize = TextureLoader::getInstance().getTexture(selfSprite.getSpriteID()).getSize();
    for (const auto& other: registry.getEntities()) {
        if (self == other)
            continue;
        if (!registry.contains<Tags>(other) || !registry.contains<Position>(other) || !registry.contains<Sprite>(other)) {
            continue;
        }
        if (registry.contains<network::Client>(other) && tags.hasTag(Tag::Ally)) {
            continue;
        }
        ecs::component::Tags otherTags = registry.getComponent<Tags>(other);
        if (tags == otherTags || otherTags.hasTag(Tag::Explosion)) {
            continue;
        }
        if (tags.hasTag(Tag::Bullet) && otherTags.hasTag(Tag::Bullet))
            continue;
        auto& otherPosition = registry.getComponent<Position>(other);
        auto& otherSprite = registry.getComponent<Sprite>(other);
        std::pair<int, int> otherSize = TextureLoader::getInstance().getTexture(otherSprite.getSpriteID()).getSize();
        switch (algo) {
            case CollisionAlgorithm::AABB:

                if (checkAABBCollision(selfPosition, selfSize, otherPosition, otherSize)) {
                    EntitySchematic::createExplosion(other);
                    registry.removeEntity(self);
                    registry.removeEntity(other);
                }
                break;
            case CollisionAlgorithm::Circle:
                if (checkCircleCollision(selfPosition, selfSize, otherPosition, otherSize)) {
                    EntitySchematic::createExplosion(other);
                    registry.removeEntity(self);
                    registry.removeEntity(other);
                }
                break;
            case CollisionAlgorithm::Custom:
                if (customCollision) {
                    customCollision(self, other);
                }
                break;
            default:
                break;
        }
    }
    return;
}
