#pragma once

#include "Registry.hpp"
#include "socket/Client.hpp"
#include <memory>
#include <mutex>

namespace ecs {
    class RegistryManager {
        private:
            static std::unique_ptr<RegistryManager> _instance;
            static std::mutex _mutex;

            explicit RegistryManager() = default;

            std::mutex _accessMutex;
            std::vector<std::shared_ptr<Registry>> _registries;
            std::unordered_map<std::shared_ptr<network::Client>, std::shared_ptr<Registry>> _bindings;

        public:
            RegistryManager(const RegistryManager& other) = delete;
            RegistryManager& operator=(const RegistryManager& other) = delete;

            static RegistryManager& getInstance();

            std::shared_ptr<Registry> getRegistry(int id);
            std::vector<std::shared_ptr<Registry>> getRegistries();
            std::shared_ptr<Registry> createRegistry();

            void bind(std::shared_ptr<network::Client> client, int id);
            void unbind(std::shared_ptr<network::Client> client);
            std::optional<std::shared_ptr<Registry>> get(std::shared_ptr<network::Client> client);
    };

    std::optional<ecs::Entity> getEntity(std::shared_ptr<network::Client> client);
}
