#pragma once

#include <cstdint>
#include <functional>
#include <memory>

namespace ecs {
    class Registry;

    class Entity {
        private:
            uint64_t _id;
            std::shared_ptr<Registry> _registry;

        public:
            explicit Entity(uint64_t id, std::shared_ptr<Registry> registry);
            Entity(const Entity& other) = default;

            [[nodiscard]] uint64_t getID() const;

            bool operator==(const Entity& other) const;
            bool operator<(const Entity& other) const;

            [[nodiscard]] std::shared_ptr<Registry> getRegistry() const;
    };
}

template <>
struct std::hash<ecs::Entity> {
        std::size_t operator()(const ecs::Entity& key) const noexcept {
            return std::hash<std::size_t>()(key.getID());
        }
};
