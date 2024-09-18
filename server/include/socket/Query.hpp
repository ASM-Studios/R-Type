#pragma once

#include <boost/asio.hpp>

namespace network::socket::udp {
    class Query {
    private:
        std::string _hostname;
        int _port;
        boost::asio::io_service _ioService;
        boost::asio::ip::udp::resolver _resolver;
        boost::asio::ip::udp::resolver::query _query;
        boost::asio::ip::udp::endpoint _endpoint;
        boost::asio::ip::udp::socket _socket;

    public:
        explicit Query(std::string hostname, int port, std::string message);
        ~Query() = default;
    };
}
