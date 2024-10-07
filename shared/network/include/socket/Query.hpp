#pragma once

#include "query/RawRequest.hpp"
#include <boost/asio.hpp>

namespace network::socket::udp {
    class QuerySender {
    private:
        std::string _hostname;
        int _port;
        boost::asio::io_service _ioService;
        boost::asio::ip::udp::resolver _resolver;
        boost::asio::ip::udp::resolver::query _query;
        boost::asio::ip::udp::endpoint _endpoint;
        boost::asio::ip::udp::socket _socket;

    public:
        explicit QuerySender(std::string hostname, int port);
        ~QuerySender() = default;

        void send(RawRequest request);
    };
}
