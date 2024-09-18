#include "socket/Query.hpp"
#include <string>

namespace network::socket::udp {
    Query::Query(const std::string hostname, int port, const std::string message) :
        _hostname(hostname), _port(port), _resolver(this->_ioService),
        _query(boost::asio::ip::udp::v4(), this->_hostname, std::to_string(this->_port)),
        _endpoint(*_resolver.resolve(this->_query)), _socket(this->_ioService) {
        this->_socket.open(boost::asio::ip::udp::v4());
        this->_socket.send_to(boost::asio::buffer(message), this->_endpoint);
    }
}
