#include "Client.hpp"
#include "Core.hpp"
#include "Entity.hpp"
#include "EntitySchematic.hpp"
#include "Logger.hpp"
#include "RegistryManager.hpp"
#include "Tags.hpp"
#include "query/Header.hpp"
#include "query/Payloads.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"
#include "socket/ServerManager.hpp"
#include <boost/asio/buffer.hpp>
#include <csignal>
#include <cstdio>
#include <iomanip>
#include <iostream>

ecs::Entity registerClientEntity(network::Client client);

void handleInput(network::Client client, RawRequest request) {
    ecs::Entity entity = registerClientEntity(client);
    TypedQuery<ecs::component::Input> query = request.getQuery();
    ecs::RegistryManager::getInstance().getRegistry().setComponent(entity, query.getPayload());
}

const std::map<RequestType, void (*)(network::Client client, RawRequest rawRequest)> requestAction = {
    {RequestType::INPUT, &handleInput}};

void hexDisplay(const char *ptr, std::size_t n) {
    for (int i = 0; i < n; i++) {
        if ((i > 0) && (i % 20 == 0)) {
            std::cout << std::endl;
        }
        std::cout << std::setbase(16) << std::setw(2) << std::setfill('0') << static_cast<int>(ptr[i] & 0xff) << " ";
    }
    std::cout << std::endl
              << std::endl;
}

int main(int ac, char **av) {
    auto args = std::span<char *>(av, ac);
    Core core;

    core.init(args);
    return core.run();
}
