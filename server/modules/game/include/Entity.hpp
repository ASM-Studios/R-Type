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

        [[nodiscard]] std::size_t getID() const {
            return this->_id;
        }

        bool operator==(const Entity& other) const {
            return this->_id == other._id;
        }

        template <typename Component>
        void addComponent();

        template <typename... Components>
        void addComponents();

        template <typename Component>
        void setComponent(Component component);

        template <typename Component>
        [[nodiscard]] bool contains() const;

        template <typename Component>
        std::optional<std::reference_wrapper<Component>> getComponent();

        template <typename Component>
        [[nodiscard]] std::optional<Component> getComponent() const;

        void reset();
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

#include "Entity.inl"
