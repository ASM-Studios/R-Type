#include "socket/Query.hpp"
#include <string>

namespace network::socket::udp {
    QuerySender::QuerySender(const std::string hostname, int port) :
        _hostname(hostname), _port(port), _resolver(this->_ioService),
        _query(boost::asio::ip::udp::v4(), this->_hostname, std::to_string(this->_port)),
        _endpoint(*_resolver.resolve(this->_query)), _socket(this->_ioService) {
        this->_socket.open(boost::asio::ip::udp::v4());
    }

    void QuerySender::send(RawRequest request) {
        this->_socket.send_to(boost::asio::buffer(request), this->_endpoint);
    }
}
