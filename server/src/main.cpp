#include "Client.hpp"
#include "Core.hpp"
#include "Player.hpp"
#include "RegistryManager.hpp"
#include "Texture.hpp"
#include "query/Header.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"
#include <cstdio>
#include <iomanip>
#include <iostream>

static ecs::Entity registerClientEntity(network::Client client) {
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    for (const auto& entity: registry.getEntities()) {
        if (registry.contains<network::Client>(entity, client)) {
            return entity;
        }
    }
    ecs::Entity entity = registry.createEntity<>(0);
    registry.setComponent(entity, client);
    return entity;
}

const std::map<RequestType, void (*)(network::Client client, RawRequest rawRequest)> requestAction = {
    {NOTHING, [](network::Client client, RawRequest rawRequest) {
         registerClientEntity(client);
     }}};

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
