#include "RegistryManager.hpp"
#include "Logger.hpp"
#include <mutex>

namespace ecs {
    RegistryManager& RegistryManager::getInstance() {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_instance == nullptr) {
            _instance = std::unique_ptr<RegistryManager>(new RegistryManager);
        }
        return *_instance;
    }

    Registry& RegistryManager::getRegistry() {
        return this->_registry;
    }

    std::unique_ptr<RegistryManager> RegistryManager::_instance(nullptr);
    std::mutex RegistryManager::_mutex;
}
