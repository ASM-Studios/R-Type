#include "socket/Server.hpp"
#include <boost/array.hpp>

namespace udp_socket {
    Server::Server(int port) :
        _endpoint(udp::endpoint(udp::endpoint(udp::v4(), port))),
        _socket(this->_ioService, this->_endpoint) {}

    bool Server::availableRequest() {
        return this->_socket.available() > 0;
    }
}
