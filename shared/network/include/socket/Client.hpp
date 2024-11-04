#pragma once

#include <boost/asio.hpp>
#include <exception>
#include <functional>
#include <optional>
#include <sys/types.h>

namespace network {
    class Client {
        public:
            class NotInitialized : public std::exception {
                private:
                    std::string _message;

                public:
                    explicit NotInitialized();
                    ~NotInitialized() override = default;

                    [[nodiscard]] const char *what() const noexcept override;
            };

        private:
            std::optional<boost::asio::ip::address_v4> _udpIP;
            std::optional<boost::asio::ip::port_type> _udpPort;

            boost::asio::ip::tcp::socket _tcpSocket;

        public:
            explicit Client(std::string hostname, int tcpPort, int udpPort); // FOR CLIENT
            explicit Client(boost::asio::ip::tcp::socket& socket);           // FOR SERVER
            ~Client() = default;

            bool operator==(const Client& other) const;

            void read();
            void init(boost::asio::ip::address_v4 udpIP, boost::asio::ip::port_type udpPort);

            [[nodiscard]] uint64_t getID() const;
            [[nodiscard]] boost::asio::ip::address_v4 getUdpIP() const;
            [[nodiscard]] boost::asio::ip::port_type getUdpPort() const;
            [[nodiscard]] boost::asio::ip::tcp::socket& getSocket();
    };
}

std::ostream& operator<<(std::ostream& os, const network::Client& client);
