#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ip/basic_endpoint.hpp>
#include <unordered_map>

namespace network::socket::udp {
    class Server {
    private:
        boost::asio::io_service _ioService;
        boost::asio::ip::udp::endpoint _endpoint;
        boost::asio::ip::udp::socket _socket;

    public:
        explicit Server(int port);
        ~Server() = default;

        template <typename T>
        [[nodiscard]] T recv() {
            std::array<T, 1> recv_buf = {0};
            boost::asio::ip::udp::endpoint remoteEndpoint;
            boost::system::error_code error;
            this->_socket.receive_from(boost::asio::buffer(recv_buf), remoteEndpoint, 0, error);
            if (error) {
                throw boost::system::system_error(error);
            }
            return recv_buf[0];
        }

        bool availableRequest();
    };
}
