#include "socket/Server.hpp"
#include "socket/NRegistry.hpp"
#include "socket/NetworkManager.hpp"
#include <boost/array.hpp>
#include <memory>

namespace network::socket::udp {
    Server::Server(boost::asio::io_context& context) :
        _socket(context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0)) {}

    Server::Server(boost::asio::io_context& context, int port) :
        _socket(context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)) {}

    void Server::read() {
        auto request = std::make_shared<RawRequest>();
        auto remoteEndpoint = std::make_shared<boost::asio::ip::udp::endpoint>();

        this->_socket.async_receive_from(boost::asio::buffer(request.get(), sizeof(RawRequest)), *remoteEndpoint, [this, request, remoteEndpoint](const boost::system::error_code& error, std::size_t bytes) {
            if (error) {
                Logger::log(LogLevel::ERR, error.message());
                return;
            }
            this->read();
            {
                Singleton<network::Registry>::getInstance().lock();
                auto client = Singleton<network::Registry>::getInstance().get().getClient(request->getUuid());
                if (client.has_value()) {
                    client->get()->init(remoteEndpoint->address().to_v4(), remoteEndpoint->port());
                    QueryHandler::getInstance().addUdpQuery({client.value(), *request});
                } else {
                    Logger::log(LogLevel::WARNING, "Client must be init before sending udp request");
                }
                Singleton<network::Registry>::getInstance().unlock();
            }
        });
    }

    boost::asio::ip::udp::socket& Server::getSocket() {
        return this->_socket;
    }
}

namespace network::socket::tcp {
    Server::Server(boost::asio::io_context& context, int port) :
        _acceptor(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {

        this->_acceptor.listen();
    }

    void Server::read() {
        this->_acceptor.async_accept([this](const boost::system::error_code& error, boost::asio::ip::tcp::socket socket) {
            if (error) {
                Logger::log(LogLevel::ERR, error.message());
                return;
            }
            this->read();
            {
                Singleton<network::Registry>::getInstance().lock();
                auto client = std::make_shared<network::Client>(socket);
                Singleton<network::Registry>::getInstance().get().addClient(client);
                Client::read(client);
                Singleton<network::Registry>::getInstance().unlock();
            }
        });
    }

    boost::asio::ip::tcp::acceptor& Server::getAcceptor() {
        return this->_acceptor;
    }
}
