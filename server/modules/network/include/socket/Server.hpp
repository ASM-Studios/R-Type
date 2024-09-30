#pragma once

#include "../Client.hpp"
#include <boost/asio.hpp>
#include <set>
#include <utility>

namespace network::socket::udp {
    class Server {
    private:
        boost::asio::io_service _ioService;
        boost::asio::ip::udp::endpoint _endpoint;
        boost::asio::ip::udp::socket _socket;
        std::set<network::Client> _clients;

        Client _registerClient(boost::asio::ip::udp::endpoint endpoint);

    public:
        explicit Server(int port);
        ~Server() = default;

        template <typename T>
        [[nodiscard]] std::pair<Client, T> recv() {
            std::array<T, 1> recv_buf = {0};
            boost::asio::ip::udp::endpoint remoteEndpoint;
            boost::system::error_code error;

            this->_socket.receive_from(boost::asio::buffer(recv_buf), remoteEndpoint, 0, error);
            if (error) {
                throw boost::system::system_error(error);
            }
            Client client = this->_registerClient(remoteEndpoint);
            return {client, recv_buf[0]};
        }

        bool availableRequest();
    };
}
