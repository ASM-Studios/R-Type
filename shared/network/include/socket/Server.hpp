#pragma once

#include "../Client.hpp"
#include "QueryHandler.hpp"
#include "query/RawRequest.hpp"
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <set>
#include <utility>

namespace network::socket::udp {
    class Server {
        private:
            boost::asio::io_context _context;
            boost::asio::ip::udp::socket _socket;

        public:
            explicit Server();
            explicit Server(int port);
            ~Server() = default;

            void read();
            void send(std::string hostname, int port, RawRequest request);

            boost::asio::io_context& getContext() {
                return this->_context;
            }

            boost::asio::ip::udp::socket& getSocket() {
                return this->_socket;
            }
    };
}
