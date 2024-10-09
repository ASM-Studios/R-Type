#pragma once

#include <vector>
#include "Entity.hpp"
#include "Position.hpp"
#include "RegistryManager.hpp"
#include "TextureLoader.hpp"

class Collision {
    public:
        Collision();
        static void updateCollisions();

    private:
        static void checkCollisions(const std::vector<std::pair<int16_t, ecs::Entity>>& sortedEntities);
        static std::vector<std::pair<int16_t, ecs::Entity>> sortedEntitiesVec();
        static bool intersects(const ecs::Entity& entity1, const ecs::Entity& entity2);
};
