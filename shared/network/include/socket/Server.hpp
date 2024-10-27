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
            void send(const Client& client, RawRequest request);
            void sendAll(std::vector<Client> clients, RawRequest request);

            boost::asio::io_context& getContext();
            boost::asio::ip::udp::socket& getSocket();
    };
}

static inline network::Client getServer() {
    const Config& config = Config::getInstance("server/config.json");
    std::string hostname = config.get("hostname").value_or("127.0.0.1");
    int port = std::stoi(config.get("port").value_or("8080"));
    return network::Client(boost::asio::ip::address_v4::from_string(hostname), port);
}
