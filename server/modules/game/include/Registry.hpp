#pragma once

#include <any>
#include <cstddef>
#include <functional>
#include <optional>
#include <set>
#include <typeindex>
#include <unordered_map>

namespace ecs {
    class Entity;

    class Registry {
    private:
        static std::size_t _maxId;
        static std::set<Entity> _entities;
        static std::unordered_map<std::type_index, std::unordered_map<Entity, std::any>> _components;

    public:
        Registry() = delete;

        template <typename... Components>
        static Entity createEntity();

        [[nodiscard]] static std::size_t getMaxEntity();
        [[nodiscard]] static std::set<Entity> getEntities();

        template <typename Component>
        static void addComponent(const Entity& entity);
        template <typename... Components>
        static void addComponents(const Entity& entity);

        template <typename Component>
        static void setComponent(const Entity& entity, Component component);

        template <typename Component>
        static bool contains(const Entity& entity);

        template <typename Component>
        static std::optional<std::reference_wrapper<Component>> getComponent(const Entity& entity);

        static void reset(const Entity& entity);
    };
}

#include "Registry.inl"
