#pragma once

#include "Entity.hpp"
#include "RegistryManager.hpp"
#include "Tags.hpp"
#include "TextureLoader.hpp"
#include "EntitySchematic.hpp"
#include <functional>

namespace ecs::component {

    enum class CollisionAlgorithm {
        AABB,
        Circle,
        Custom
    };

    struct Collision {
            CollisionAlgorithm algo;
            void checkCollision(const ecs::Entity& self);
            std::function<void (const ecs::Entity self, const ecs::Entity other)>customCollision;
    };
}

