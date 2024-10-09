#pragma once

#include "Registry.hpp"
#include <memory>
#include <mutex>

namespace ecs {
    class RegistryManager {
    private:
        static std::unique_ptr<RegistryManager> _instance;
        static std::mutex _mutex;

        explicit RegistryManager() = default;

        Registry _registry;

    public:
        RegistryManager(const RegistryManager& other) = delete;
        RegistryManager& operator=(const RegistryManager& other) = delete;

        static RegistryManager& getInstance();

        Registry& getRegistry();
    };
}
