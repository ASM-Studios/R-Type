#pragma once

#include "../Client.hpp"
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <set>
#include <utility>

namespace network::socket::udp {
    class Server {
        private:
            boost::asio::io_context _context;
            boost::asio::ip::udp::socket _socket;
            std::set<network::Client> _clients;

            Client _registerClient(boost::asio::ip::udp::endpoint endpoint);

        public:
            explicit Server();
            explicit Server(int port);
            ~Server() = default;

            template <typename T>
            [[nodiscard]] std::pair<Client, T> recv() {
                T buffer;
                boost::asio::ip::udp::endpoint remoteEndpoint;
                boost::system::error_code error;

                this->_socket.receive_from(boost::asio::buffer(&buffer, sizeof(T)), remoteEndpoint, 0, error);
                if (error) {
                    throw boost::system::system_error(error);
                }
                Client client = this->_registerClient(remoteEndpoint);
                return {client, buffer};
            }

            template <typename T>
            void send(std::string hostname, int port, T payload) {
                std::array<T, 1> arr;
                arr[0] = payload;
                boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string(hostname), port);
                this->_socket.send_to(boost::asio::buffer(arr), endpoint);
            }

            bool availableRequest();
    };
}
