#pragma once

#include "Position.hpp"
#include "Sprite.hpp"
#include "Texture.hpp"
#include <any>
#include <cstddef>
#include <cstdint>
#include <set>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace ecs {
    class Entity;

    class Registry {
        public:
            class AlreadyExist final : public std::exception {
                private:
                    std::string _message;

                public:
                    explicit AlreadyExist(uint64_t id);
                    ~AlreadyExist() override = default;

                    [[nodiscard]] const char *what() const noexcept override;
            };

            class ComponentNotFound final : public std::exception {
                private:
                    std::string _message;

                public:
                    explicit ComponentNotFound(const std::string& componentName);
                    ~ComponentNotFound() override = default;

                    [[nodiscard]] const char *what() const noexcept override;
            };

            int _generateID();

        private:
            uint8_t _id;
            std::mutex _mutex;
            std::set<Entity> _entities;
            std::unordered_map<std::type_index, std::unordered_map<Entity, std::any>> _components;

        public:
            explicit Registry(uint8_t id = 0);

            template <typename... Components>
            Entity createEntity();
            template <typename... Components>
            Entity createEntity(uint64_t id);

            [[nodiscard]] std::set<Entity> getEntities();
            template <typename Component>
            [[nodiscard]] std::unordered_map<Entity, Component> getEntities();

            void removeEntity(const Entity& entity);
            bool isEntityValid(const Entity& entity);

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
            template <typename Component>
            [[nodiscard]] std::vector<Component> getComponents();

            [[nodiscard]] std::size_t getNoComponent(const Entity& entity);

            void reset(const Entity& entity);
            void resetAll();
    };
}

#include "Registry.inl"
