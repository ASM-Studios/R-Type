#pragma once

#include <any>
#include <cstddef>
#include <cstdint>
#include <set>
#include <string>
#include <typeindex>
#include <unordered_map>

#include "Position.hpp"
#include "Sprite.hpp"
#include "AI.hpp"
#include "Texture.hpp"

namespace ecs {
    class Entity;

    class Registry {
    public:
        class ComponentNotFound final : public std::exception {
        private:
            std::string _message;

        public:
            explicit ComponentNotFound(const std::string& componentName);
            ~ComponentNotFound() override = default;

            [[nodiscard]] const char *what() const noexcept override;
        };

    private:
        uint8_t _id;
        uint32_t _maxId;
        std::set<Entity> _entities;
        std::unordered_map<std::type_index, std::unordered_map<Entity, std::any>> _components;

    public:
        explicit Registry(uint8_t id = 0);

        template <typename... Components>
        Entity createEntity();

        [[nodiscard]] std::size_t getMaxEntity() const;
        [[nodiscard]] std::set<Entity> getEntities();

        template <typename Component>
        void addComponent(const Entity& entity);
        template <typename... Components>
        void addComponents(const Entity& entity);

        template <typename Component>
        void setComponent(const Entity& entity, Component component);

        template <typename Component>
        [[nodiscard]] bool contains(const Entity& entity);
        template <typename Component>
        [[nodiscard]] bool contains(const Entity& entity, const Component& component);

        template <typename Component>
        [[nodiscard]] Component& getComponent(const Entity& entity);

        void reset(const Entity& entity);
    };
}

#include "Registry.inl"
