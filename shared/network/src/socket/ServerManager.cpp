#include "socket/ServerManager.hpp"
#include "Logger.hpp"
#include <format>
#include <mutex>

namespace network::socket::udp {
    ServerManager::NotInitialized::NotInitialized() :
        _message(std::format("Server must be initialized")) {}

    const char *ServerManager::NotInitialized::what() const noexcept {
        return this->_message.c_str();
    }

    ServerManager::ServerManager() :
        _server(std::nullopt) {}

    ServerManager& ServerManager::getInstance() {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_instance == nullptr) {
            _instance = std::unique_ptr<ServerManager>(new ServerManager());
        }
        return *_instance;
    }

    void ServerManager::init() {
        this->_server.emplace();
    }

    void ServerManager::init(int port) {
        this->_server.emplace(port);
    }

    Server& ServerManager::getServer() {
        if (!this->_server.has_value()) {
            throw NotInitialized();
        }
        return this->_server.value();
    }

    std::unique_ptr<ServerManager> ServerManager::_instance;
    std::mutex ServerManager::_mutex;
}
