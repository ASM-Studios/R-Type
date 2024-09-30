#include "socket/Server.hpp"
#include <boost/array.hpp>

namespace network::socket::udp {
    Client Server::_registerClient(boost::asio::ip::udp::endpoint endpoint) {
        Client client(endpoint.address().to_v4(), endpoint.port());
        this->_clients.insert(client);
        return client;
    }

    Server::Server(int port) :
        _endpoint(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))),
        _socket(this->_ioService, this->_endpoint) {}

    bool Server::availableRequest() {
        return this->_socket.available() > 0;
    }
}
