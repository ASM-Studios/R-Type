#pragma once

#include "Client.hpp"
#include "QueryHandler.hpp"
#include "Singleton.hpp"
#include "query/RawRequest.hpp"
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <functional>
#include <set>
#include <utility>

namespace network::socket::udp {
    class Server {
        private:
            boost::asio::ip::udp::socket _socket;

        public:
            explicit Server(boost::asio::io_context& context);
            explicit Server(boost::asio::io_context& context, int port);
            ~Server() = default;

            void read();

            boost::asio::ip::udp::socket& getSocket();
    };
}

namespace network::socket::tcp {
    class Server {
        private:
            boost::asio::ip::tcp::acceptor _acceptor;

        public:
            explicit Server(boost::asio::io_context& context, int port);
            ~Server() = default;

            void read();

            boost::asio::ip::tcp::acceptor& getAcceptor();
    };
}

static inline Singleton<std::shared_ptr<network::Client>>& initServer() {
    const Config& config = Config::getInstance("client/config.json");
    std::string online = config.get("online").value_or("true");
    if (online == "true") {
        std::string hostname = config.get("hostname").value_or("127.0.0.1");
        int udpPort = std::stoi(config.get("udp_port").value_or("8080"));
        int tcpPort = std::stoi(config.get("tcp_port").value_or("8081"));
        auto client = std::make_shared<network::Client>(hostname, tcpPort, udpPort);
        network::Client::read(client);
        return Singleton<std::shared_ptr<network::Client>>::wrap(client);
    }
    return Singleton<std::shared_ptr<network::Client>>::wrap(std::make_shared<network::Client>());
}

static inline Singleton<std::shared_ptr<network::Client>>& getServer() {
    return Singleton<std::shared_ptr<network::Client>>::getInstance();
}
