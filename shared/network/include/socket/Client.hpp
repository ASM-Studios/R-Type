#pragma once

#include <boost/asio.hpp>
#include <exception>
#include <functional>
#include <optional>
#include <sys/types.h>

namespace network {
    class Client {
        private:
            std::optional<boost::asio::ip::address_v4> _udpIP;
            std::optional<boost::asio::ip::port_type> _udpPort;

            std::optional<boost::asio::ip::tcp::socket> _tcpSocket;

            std::mutex _accessMutex;

        public:
            explicit Client();
            explicit Client(std::string hostname, int tcpPort, int udpPort); // FOR CLIENT
            explicit Client(boost::asio::ip::tcp::socket& socket);           // FOR SERVER
            ~Client() = default;

            bool operator==(const Client& other);

            static void read(std::shared_ptr<Client> client);
            void init(boost::asio::ip::address_v4 udpIP, boost::asio::ip::port_type udpPort);

            [[nodiscard]] uint64_t getID();
            [[nodiscard]] std::optional<boost::asio::ip::address_v4> getUdpIP();
            [[nodiscard]] std::optional<boost::asio::ip::port_type> getUdpPort();
            [[nodiscard]] std::optional<std::reference_wrapper<boost::asio::ip::tcp::socket>> getSocket();
    };
}
