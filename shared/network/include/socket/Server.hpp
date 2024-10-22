#pragma once

#include "../Client.hpp"
#include "QueryHandler.hpp"
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
            void run();

            template <typename T>
            [[nodiscard]] std::pair<Client, T> recv() {
                T buffer;
                boost::asio::ip::udp::endpoint remoteEndpoint;
                boost::system::error_code error;

                this->_socket.receive_from(boost::asio::buffer(&buffer, sizeof(T)), remoteEndpoint, 0, error);
                if (error) {
                    throw boost::system::system_error(error);
                }
                Client client(remoteEndpoint.address().to_v4(), remoteEndpoint.port());
                return {client, buffer};
            }

            template <typename T>
            void asyncRecv() {
                auto buffer = std::make_shared<T>();
                auto remoteEndpoint = std::make_shared<boost::asio::ip::udp::endpoint>();
                Logger::log(LogLevel::INFO, "Waiting for data");
                this->_socket.async_receive_from(
                    boost::asio::buffer(buffer.get(), sizeof(T)),
                    *remoteEndpoint,
                    [this, buffer, remoteEndpoint](boost::system::error_code error, std::size_t bytesReceived) {
                        if (!error && bytesReceived > 0) {
                            Logger::log(LogLevel::INFO, "Received data");
                            Client client(remoteEndpoint->address().to_v4(), remoteEndpoint->port());
                            RawRequest request = *buffer;
                            network::QueryHandler::getInstance().addQuery({client, request});
                        } else {
                            Logger::log(LogLevel::ERR, std::format("Receive error: {0}", error.message()));
                        }
                        asyncRecv<T>();
                    }
                );
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
