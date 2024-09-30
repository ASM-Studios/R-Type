#pragma once

#include <functional>
#include <optional>

namespace ecs {
    class Registry;

    class Entity {
    private:
        std::size_t _id;

    public:
        explicit Entity(std::size_t id);

        [[nodiscard]] std::size_t getID() const;

        bool operator==(const Entity& other) const;
    };
}

template <>
struct std::hash<ecs::Entity> {
    std::size_t operator()(const ecs::Entity& key) const {
        return std::hash<std::size_t>()(key.getID());
    }
};

template <>
struct std::less<ecs::Entity> {
    bool operator()(const ecs::Entity& left, const ecs::Entity& right) const {
        return left.getID() < right.getID();
    }
};
