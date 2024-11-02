#include "RegistryManager.hpp"
#include "Logger.hpp"
#include "Registry.hpp"
#include <mutex>

namespace ecs {
    RegistryManager& RegistryManager::getInstance() {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_instance == nullptr) {
            _instance = std::unique_ptr<RegistryManager>(new RegistryManager);
        }
        return *_instance;
    }

    std::shared_ptr<Registry> RegistryManager::getRegistry(int id) {
        std::lock_guard<std::mutex> _lock(this->_accessMutex);
        return this->_registries.at(id);
    }

    std::vector<std::shared_ptr<Registry>> RegistryManager::getRegistries() {
        return this->_registries;
    }

    std::shared_ptr<Registry> RegistryManager::createRegistry() {
        std::lock_guard<std::mutex> _lock(this->_accessMutex);
        auto registry = std::make_shared<Registry>(this->_registries.size());
        this->_registries.push_back(registry);
        return registry;
    }

    void RegistryManager::bind(std::shared_ptr<network::Client> client, int id) {
        std::lock_guard<std::mutex> _lock(this->_accessMutex);
        this->_bindings.insert({client, this->_registries[id]});
    }

    void RegistryManager::unbind(std::shared_ptr<network::Client> client) {
        std::lock_guard<std::mutex> _lock(this->_accessMutex);
        this->_bindings.erase(client);
    }

    std::optional<std::shared_ptr<Registry>> RegistryManager::get(std::shared_ptr<network::Client> client) {
        std::lock_guard<std::mutex> _lock(this->_accessMutex);
        if (this->_bindings.contains(client)) {
            Logger::log(LogLevel::INFO, "Found client");
            return this->_bindings.at(client);
        }
        Logger::log(LogLevel::ERR, "Client not found");
        return std::nullopt;
    }

    std::unique_ptr<RegistryManager> RegistryManager::_instance(nullptr);
    std::mutex RegistryManager::_mutex;

    std::optional<ecs::Entity> getEntity(std::shared_ptr<network::Client> client) {
        auto registry = RegistryManager::getInstance().get(client);
        if (!registry.has_value()) {
            return std::nullopt;
        }
        return Entity(client->getID(), registry.value());
    }
}
