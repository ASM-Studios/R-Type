#pragma once

#include <functional>

namespace ecs {
    class Registry;

    class Entity {
    private:
        std::size_t _id;
        std::reference_wrapper<Registry> _registry;

    public:
        explicit Entity(std::size_t id, Registry& registry);
        Entity(const Entity& other) = default;

        [[nodiscard]] std::size_t getID() const;

        bool operator==(const Entity& other) const;
        bool operator<(const Entity& other) const;
    };
}

template <>
struct std::hash<ecs::Entity> {
    std::size_t operator()(const ecs::Entity& key) const {
        return std::hash<std::size_t>()(key.getID());
    }
};
