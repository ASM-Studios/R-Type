#include "socket/Client.hpp"
#include "Logger.hpp"
#include "socket/NRegistry.hpp"
#include "socket/NetworkManager.hpp"
#include <boost/asio.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <cstdint>
#include <iostream>
#include <memory>

namespace network {
    Client::NotInitialized::NotInitialized() :
        _message(std::format("UDP Client is not init")) {}

    const char *Client::NotInitialized::NotInitialized::what() const noexcept {
        return this->_message.c_str();
    }

    Client::Client(std::string hostname, int tcpPort, int udpPort) :
        _udpIP(boost::asio::ip::address_v4::from_string(hostname)),
        _udpPort(udpPort),
        _tcpSocket(socket::NetworkManager::getInstance().getContext()) {

        try {
            this->_tcpSocket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::from_string(hostname), tcpPort));
            Logger::log(LogLevel::INFO, "Connected");
        } catch (std::exception& e) {
            Logger::log(LogLevel::ERR, e.what());
        }
    }

    Client::Client(boost::asio::ip::tcp::socket& socket) :
        _udpIP(std::nullopt),
        _udpPort(std::nullopt),
        _tcpSocket(std::move(socket)) {}

    bool Client::operator==(const Client& other) const {
        return this->_udpIP == other._udpIP && this->_udpPort == other._udpPort;
    }

    void Client::read() {
        auto *request = new RawRequest();                            // NOLINT
        auto *remoteEndpoint = new boost::asio::ip::udp::endpoint(); // NOLINT

        this->_tcpSocket.async_receive(boost::asio::buffer(request, sizeof(RawRequest)), [this, request, remoteEndpoint](auto error, auto bytes) {
            if (error) {
                Logger::log(LogLevel::ERR, error.what());
                return;
            }
            this->read();
            {
                Singleton<network::Registry>::getInstance().lock();
                Singleton<network::Registry>::getInstance().get().registerClient(this, request->getUuid());

                auto client = Singleton<network::Registry>::getInstance().get().getClient(request->getUuid());
                if (client.has_value()) {
                    QueryHandler::getInstance().addTcpQuery({client.value(), *request});
                }
                Singleton<network::Registry>::getInstance().unlock();
            }
            delete request;        // NOLINT
            delete remoteEndpoint; // NOLINT
        });
    }

    void Client::init(boost::asio::ip::address_v4 udpIP, boost::asio::ip::port_type udpPort) {
        this->_udpIP.emplace(udpIP);
        this->_udpPort.emplace(udpPort);
    }

    uint64_t Client::getID() const {
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

    boost::asio::ip::address_v4 Client::getUdpIP() const {
        if (!this->_udpIP.has_value()) {
            throw NotInitialized();
        }
        return this->_udpIP.value();
    }

    boost::asio::ip::port_type Client::getUdpPort() const {
        if (!this->_udpPort.has_value()) {
            throw NotInitialized();
        }
        return this->_udpPort.value();
    }

    boost::asio::ip::tcp::socket& Client::getSocket() {
        return this->_tcpSocket;
    }
}

std::ostream& operator<<(std::ostream& os, const network::Client& client) {
    os << client.getUdpIP() << " - " << client.getUdpPort();
    return os;
}
