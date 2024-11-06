#pragma once

#include <boost/asio.hpp>
#include "Entity.hpp"
#include "RegistryManager.hpp"
#include "Server.hpp"
#include <boost/uuid/uuid.hpp>
#include <exception>
#include <mutex>
#include <vector>

namespace network::socket {
    enum class Mode {
        TCP,
        UDP
    };

    class NetworkManager {
        private:
            boost::asio::io_context _ioContext;
            static std::unique_ptr<NetworkManager> _instance;
            static std::mutex _mutex;

            explicit NetworkManager();

            std::optional<udp::Server> _udpServer;
            std::optional<tcp::Server> _tcpServer;

        public:
            NetworkManager(const NetworkManager& other) = delete;
            NetworkManager& operator=(const NetworkManager& other) = delete;
            ~NetworkManager() = default;

            static NetworkManager& getInstance();

            void init();                           // AS CLIENT
            void init(int _udpPort, int _tcpPort); // AS SERVER

            void send(std::shared_ptr<Client> client, RawRequest request, Mode mode);
            void sendAll(std::vector<std::shared_ptr<Client>> clients, RawRequest request, Mode mode);

            boost::asio::io_context& getContext();
    };

    void disconnectClient(std::shared_ptr<Client> client);
}
