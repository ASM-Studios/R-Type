#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <sys/types.h>

namespace network {
    class Client {
    private:
        boost::asio::ip::address_v4 _ip;
        boost::asio::ip::port_type _port;

    public:
        explicit Client(const boost::asio::ip::address_v4& ip, const boost::asio::ip::port_type& port);
        ~Client() = default;

        bool operator==(const Client& other) const;

        [[nodiscard]] uint64_t getID() const;
        [[nodiscard]] boost::asio::ip::address_v4 getIP() const;
        [[nodiscard]] boost::asio::ip::port_type getPort() const;
    };
}

std::ostream& operator<<(std::ostream& os, const network::Client& client);

template <>
struct std::less<network::Client> {
    bool operator()(const network::Client& left, const network::Client& right) const {
        return left.getID() < right.getID();
    }
};
