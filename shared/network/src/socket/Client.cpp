#include "socket/Client.hpp"
#include "Logger.hpp"
#include "socket/NRegistry.hpp"
#include "socket/NetworkManager.hpp"
#include <boost/asio.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>

namespace network {
    Client::Client() :
        _udpIP(std::nullopt),
        _udpPort(std::nullopt),
        _tcpSocket(std::nullopt) {}

    Client::Client(std::string hostname, int tcpPort, int udpPort) :
        _udpIP(boost::asio::ip::address_v4::from_string(hostname)),
        _udpPort(udpPort),
        _tcpSocket(socket::NetworkManager::getInstance().getContext()) {

        try {
            this->_tcpSocket.value().connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(hostname), tcpPort));
            Logger::log(LogLevel::INFO, "Connected");
        } catch (std::exception& e) {
            Logger::log(LogLevel::ERR, e.what());
        }
    }

    Client::Client(boost::asio::ip::tcp::socket& socket) :
        _udpIP(std::nullopt),
        _udpPort(std::nullopt),
        _tcpSocket(std::move(socket)) {}

    bool Client::operator==(const Client& other) {
        std::lock_guard<std::mutex> lock(this->_accessMutex);
        return this->_udpIP.value() == other._udpIP.value() && this->_udpPort.value() == other._udpPort.value();
    }

    void Client::read(std::shared_ptr<Client> client) {
        if (!client->_tcpSocket.has_value()) {
            return;
        }
        auto request = std::make_shared<RawRequest>();
        auto remoteEndpoint = std::make_shared<boost::asio::ip::udp::endpoint>();

        client->_tcpSocket->async_receive(boost::asio::buffer(request.get(), sizeof(RawRequest)), [client, request, remoteEndpoint](auto error, auto bytes) {
            if (error) {
                socket::disconnectClient(client);
                Logger::log(LogLevel::ERR, error.what());
                return;
            }
            Client::read(client);
            {
                Singleton<network::Registry>::getInstance().lock();
                Singleton<network::Registry>::getInstance().get().registerClient(client, request->getUuid());

                auto client = Singleton<network::Registry>::getInstance().get().getClient(request->getUuid());
                if (client.has_value()) {
                    QueryHandler::getInstance().addTcpQuery({client.value(), *request});
                }
                Singleton<network::Registry>::getInstance().unlock();
            }
        });
    }

    void Client::init(boost::asio::ip::address_v4 udpIP, boost::asio::ip::port_type udpPort) {
        std::lock_guard<std::mutex> lock(this->_accessMutex);
        this->_udpIP.emplace(udpIP);
        this->_udpPort.emplace(udpPort);
    }

    uint64_t Client::getID() {
        std::lock_guard<std::mutex> lock(this->_accessMutex);
        if (!this->_udpIP.has_value() || !this->_udpPort.has_value()) {
            return -1;
        }
        uint64_t id = 0;

        for (std::size_t i = 0; i < 4; i++) {
            id = (id << 8) | this->_udpIP->to_bytes().at(i);
        }
        id = (id << 16) | this->_udpPort.value();
        return id;
    }

    std::optional<boost::asio::ip::address_v4> Client::getUdpIP() {
        std::lock_guard<std::mutex> lock(this->_accessMutex);
        return this->_udpIP;
    }

    std::optional<boost::asio::ip::port_type> Client::getUdpPort() {
        std::lock_guard<std::mutex> lock(this->_accessMutex);
        return this->_udpPort;
    }

    std::optional<std::reference_wrapper<boost::asio::ip::tcp::socket>> Client::getSocket() {
        std::lock_guard<std::mutex> lock(this->_accessMutex);
        if (this->_tcpSocket.has_value()) {
            return std::ref(this->_tcpSocket.value());
        }
        return std::nullopt;
    }
}
