#pragma once

#include "../../network/include/Client.hpp"
#include "Entity.hpp"

namespace ecs::component {
    struct Player {
        network::Client client;

        explicit Player(const network::Client& client) :
            client(client) {}
    };
}

namespace ecs::system {
}
