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
#include "Texture.hpp"

namespace ecs {
    class Entity;

    class Registry {
        public:
            class AlreadyExist final : public std::exception {
                private:
                    std::string _message;

                public:
                    explicit AlreadyExist(int id);
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
            std::set<Entity> _entities;
            std::unordered_map<std::type_index, std::unordered_map<Entity, std::any>> _components;

        public:
            explicit Registry(uint8_t id = 0);

            template <typename... Components>
            Entity createEntity();

            template <typename... Components>
            Entity createEntity(int id);

            [[nodiscard]] std::set<Entity> getEntities();

            void removeEntity(const Entity& entity);
            bool isEntityValid(const Entity& entity) const;

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

            [[nodiscard]] std::size_t getNoComponent(const Entity& entity);

            void display(const Entity& entity);
            void displayAll();

            void reset(const Entity& entity);
            void resetAll();
    };
}

#include "Registry.inl"
