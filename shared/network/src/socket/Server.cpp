#include "socket/Server.hpp"
#include <boost/array.hpp>

namespace network::socket::udp {
    Server::Server() :
        _socket(_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0)) {}

    Server::Server(int port) :
        _socket(_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)) {}
    bool Server::availableRequest() {
        return this->_socket.available() > 0;
    }
    void Server::run() {
        _context.run();
    }
}
