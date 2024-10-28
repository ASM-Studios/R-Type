#pragma once

#include "socket/Server.hpp"
#include <exception>
#include <mutex>

namespace network::socket {
    class ServerManager {
        private:
            static std::unique_ptr<ServerManager> _instance;
            static std::mutex _mutex;

            explicit ServerManager();

            std::optional<udp::Server> _server;

        public:
            class NotInitialized : public std::exception {
                private:
                    std::string _message;

                public:
                    explicit NotInitialized();
                    ~NotInitialized() override = default;

                    [[nodiscard]] const char *what() const noexcept override;
            };
            ServerManager(const ServerManager& other) = delete;
            ServerManager& operator=(const ServerManager& other) = delete;
            ~ServerManager() = default;

            static ServerManager& getInstance();

            void init();
            void init(int port);

            udp::Server& getServer();
    };
}
