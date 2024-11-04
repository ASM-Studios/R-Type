#include "socket/NetworkManager.hpp"
#include "Logger.hpp"
#include <format>
#include <mutex>

namespace network::socket {
    NetworkManager::NetworkManager() :
        _udpServer(std::nullopt),
        _tcpServer(std::nullopt) {}

    NetworkManager& NetworkManager::getInstance() {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_instance == nullptr) {
            _instance = std::unique_ptr<NetworkManager>(new NetworkManager());
        }
        return *_instance;
    }

    void NetworkManager::init() {
        this->_udpServer.emplace(this->_ioContext);
        this->_udpServer.value().read();
    }

    void NetworkManager::init(int udpPort, int tcpPort) {
        this->_udpServer.emplace(this->_ioContext, udpPort);
        this->_tcpServer.emplace(this->_ioContext, tcpPort);
        this->_udpServer.value().read();
        this->_tcpServer.value().read();
    }

    boost::asio::io_context& NetworkManager::getContext() {
        return this->_ioContext;
    }

    void NetworkManager::send(std::shared_ptr<Client> client, RawRequest request, Mode mode) {
        if (mode == Mode::UDP) {
            if (client->getUdpIP().has_value() && client->getUdpPort().has_value()) {
                boost::asio::ip::udp::endpoint endpoint(client->getUdpIP().value(), client->getUdpPort().value());
                this->_udpServer->getSocket().send_to(boost::asio::buffer(request), endpoint);
            }
        } else {
            auto socket = client->getSocket();
            if (socket.has_value()) {
                client->getSocket().value().get().send(boost::asio::buffer(request));
            }
        }
    }

    void NetworkManager::sendAll(std::vector<std::shared_ptr<Client>> clients, RawRequest request, Mode mode) {
        for (auto& client: clients) {
            this->send(client, request, mode);
        }
    }

    std::unique_ptr<NetworkManager> NetworkManager::_instance;
    std::mutex NetworkManager::_mutex;
}
