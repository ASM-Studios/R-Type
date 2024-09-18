#include "socket/Query.hpp"
#include <string>

namespace udp_socket {
    Query::Query(const std::string hostname, int port,
                 const std::string message) :
        _hostname(hostname), _port(port), _resolver(this->_ioService),
        _query(udp::v4(), this->_hostname, std::to_string(this->_port)),
        _endpoint(*_resolver.resolve(this->_query)), _socket(this->_ioService) {
        this->_socket.open(udp::v4());
        this->_socket.send_to(boost::asio::buffer(message), this->_endpoint);
    }
}