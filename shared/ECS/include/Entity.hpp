#pragma once

#include <functional>
#include "EntityType.hpp"

namespace ecs {
    class Registry;

    class Entity {
        private:
            std::size_t _id;
            EntityType _type;
            std::reference_wrapper<Registry> _registry;

    public:
        explicit Entity(std::size_t id, EntityType type, Registry& registry);
        Entity(const Entity& other) = default;

        [[nodiscard]] std::size_t getID() const;
        [[nodiscard]] EntityType getType() const;

        bool operator==(const Entity& other) const;
        bool operator<(const Entity& other) const;
    };
}

template <>
struct std::hash<ecs::Entity> {
    std::size_t operator()(const ecs::Entity& key) const noexcept {
        return std::hash<std::size_t>()(key.getID());
    }
};
