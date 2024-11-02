#pragma once

#include "socket/Client.hpp"
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <optional>
#include <vector>

namespace network {
    class Registry {
        private:
            std::vector<std::pair<std::optional<boost::uuids::uuid>, std::shared_ptr<network::Client>>> _clients;

        public:
            explicit Registry() = default;
            ~Registry() = default;

            void addClient(std::shared_ptr<network::Client> client);

            void registerClient(network::Client *client, boost::uuids::uuid uuid);
            void registerClient(std::shared_ptr<network::Client> client, boost::uuids::uuid uuid);

            [[nodiscard]] std::vector<std::pair<boost::uuids::uuid, std::shared_ptr<network::Client>>> getClients() const;

            std::optional<std::shared_ptr<network::Client>> getClient(boost::uuids::uuid uuid);
    };
}
