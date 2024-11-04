#include "socket/NRegistry.hpp"
#include "GameLogicMode.hpp"
#include "Logger.hpp"
#include "Singleton.hpp"
#include <memory>

namespace network {
    void Registry::addClient(std::shared_ptr<network::Client> client) {
        this->_clients.emplace_back(std::nullopt, client);
    }

    void Registry::registerClient(network::Client *client, boost::uuids::uuid uuid) {
        for (auto& [_uuid, _client]: this->_clients) {
            if (_client.get() == client) {
                _uuid = uuid;
            }
        }
    }

    void Registry::registerClient(std::shared_ptr<network::Client> client, boost::uuids::uuid uuid) {
        for (auto& [_uuid, _client]: this->_clients) {
            if (_client == client) {
                _uuid = uuid;
            }
        }
    }

    std::vector<std::pair<boost::uuids::uuid, std::shared_ptr<network::Client>>> Registry::getClients() const {
        std::vector<std::pair<boost::uuids::uuid, std::shared_ptr<network::Client>>> res;

        for (const auto& [uuid, client]: this->_clients) {
            if (uuid.has_value()) {
                res.emplace_back(uuid.value(), client);
            }
        }
        return res;
    }

    std::optional<std::shared_ptr<network::Client>> Registry::getClient(boost::uuids::uuid uuid) {
        if (uuid.is_nil()) {
            Singleton<std::shared_ptr<network::Client>>::getInstance().lock();
            auto client = Singleton<std::shared_ptr<network::Client>>::getInstance().get();
            Singleton<std::shared_ptr<network::Client>>::getInstance().unlock();
            return client;
        }
        for (auto& [_uuid, client]: this->_clients) {
            if (_uuid == uuid) {
                return client;
            }
        }
        return std::nullopt;
    }
}
