#include "socket/Server.hpp"
#include <boost/array.hpp>
#include <memory>

namespace network::socket::udp {
    Server::Server() :
        _socket(this->_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0)) {}

    Server::Server(int port) :
        _socket(_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)) {}

    void Server::read() {
        auto *request = new RawRequest(); // NOLINT
        auto *remoteEndpoint = new boost::asio::ip::udp::endpoint(); // NOLINT

        this->_socket.async_receive_from(boost::asio::buffer(request, sizeof(RawRequest)), *remoteEndpoint, [this, remoteEndpoint, request](const boost::system::error_code& error, std::size_t bytes_transferred) {
            if (error) {
                return;
            }
            Client client(remoteEndpoint->address().to_v4(), remoteEndpoint->port());
            QueryHandler::getInstance().addQuery({client, *request});
            delete request; // NOLINT
            delete remoteEndpoint; // NOLINT
            this->read();
        });
    }

    void Server::send(std::string hostname, int port, RawRequest request) {
        boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string(hostname), port);
        this->_socket.send_to(boost::asio::buffer(request), endpoint);
    }

    void Server::send(const Client& client, RawRequest request) {
        boost::asio::ip::udp::endpoint endpoint(client.getIP(), client.getPort());
        this->_socket.send_to(boost::asio::buffer(request), endpoint);
    }

    void Server::sendAll(std::vector<Client> clients, RawRequest request) {
        for (auto& client: clients) {
            this->send(client, request);
        }
    }

    boost::asio::io_context& Server::getContext() {
        return this->_context;
    }

    boost::asio::ip::udp::socket& Server::getSocket() {
        return this->_socket;
    }
}
