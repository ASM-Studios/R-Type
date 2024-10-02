#include "Client.hpp"
#include <boost/asio.hpp>
#include <cstdint>

namespace network {
    Client::Client(const boost::asio::ip::address_v4& ip, const boost::asio::ip::port_type& port) :
        _ip(std::move(ip)),
        _port(std::move(port)) {}

    bool Client::operator==(const Client& other) const {
        return this->_ip == other._ip && this->_port == other._port;
    }

    uint64_t Client::getID() const {
        uint64_t id = 0;

        for (std::size_t i = 0; i < 4; i++) {
            id = (id << 8) | this->_ip.to_bytes().at(i);
        }
        id = (id << 16) | this->_port;
        return id;
    }
}
