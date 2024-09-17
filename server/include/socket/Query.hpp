#pragma once

#include <boost/asio.hpp>

using udp = boost::asio::ip::udp;

namespace udp_socket {
    class Query {
    private:
        std::string _hostname;
        int _port;
        boost::asio::io_service _ioService;
        udp::resolver _resolver;
        udp::resolver::query _query;
        udp::endpoint _endpoint;
        udp::socket _socket;

    public:
        explicit Query(std::string hostname, int port, std::string message);
        ~Query() = default;
    };
}
