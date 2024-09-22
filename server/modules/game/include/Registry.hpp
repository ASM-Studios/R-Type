#pragma once

#include <any>
#include <cstddef>
#include <typeindex>
#include <unordered_map>

namespace ECS {
    using Entity = std::size_t;

    class Registry {
    private:
        static std::size_t _maxId;
        static std::unordered_map<std::type_index, std::unordered_map<std::size_t, std::any>> _components;

    public:
        static Entity createEntity();

        static std::size_t getMaxEntity();

        template <typename T>
        static void addComponent(Entity entity, T component) {
            _components[typeid(T)][entity] = component;
        }

        template <typename T>
        static T& getComponent(Entity entity) {
            return std::any_cast<T&>(_components[typeid(T)][entity]);
        }

        template <typename T>
        static bool contains(Entity entity) {
            return _components[typeid(T)].find(entity) != _components[typeid(T)].end();
        }
    };
}
