#pragma once

#include <any>
#include <cstddef>
#include <exception>
#include <set>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace ecs {
    class Entity;

    class Registry {
    public:
        class NoComponent : public std::exception {
        private:
            std::string _message;

        public:
            NoComponent(std::type_index index);
            ~NoComponent() override = default;

            [[nodiscard]] const char *what() const noexcept override;
        };

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
        static void addComponent(Entity entity);

        template <typename... Components>
        static void addComponents(Entity entity);

        template <typename Component>
        static bool contains(Entity entity);

        template <typename Component>
        static Component& getComponent(Entity entity);

        static void reset(Entity entity);
    };
}

#include "Registry.inl"
