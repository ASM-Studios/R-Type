#include "Entity.hpp"
#include "EntitySchematic.hpp"
#include "Logger.hpp"
#include "RegistryManager.hpp"
#include "Tags.hpp"
#include "query/Header.hpp"
#include "query/Payloads.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"
#include "socket/Client.hpp"
#include "socket/NetworkManager.hpp"
#include <boost/asio/buffer.hpp>
#include <cstdio>
#include <memory>

static void getInfo(ecs::Entity entity, std::shared_ptr<network::Client> client, ecs::Entity destEntity, std::shared_ptr<network::Client> destClient) {
    CreateEntity payload{destEntity.getID(), 23, 0, entity.getRegistry()->getComponent<ecs::component::Position>(destEntity), entity.getRegistry()->getComponent<ecs::component::Tags>(destEntity)};
    TypedQuery typedQuery(RequestType::CREATE_ENTITY, payload);
    network::socket::NetworkManager::getInstance().send(client, RawRequest(typedQuery), network::socket::Mode::UDP);
}

static void sendInfo(ecs::Entity entity, std::shared_ptr<network::Client> client, ecs::Entity destEntity, std::shared_ptr<network::Client> destClient) {
    CreateEntity payload{entity.getID(), 23, 0, entity.getRegistry()->getComponent<ecs::component::Position>(entity), entity.getRegistry()->getComponent<ecs::component::Tags>(entity)};
    TypedQuery typedQuery(RequestType::CREATE_ENTITY, payload);
    network::socket::NetworkManager::getInstance().send(destClient, RawRequest(typedQuery), network::socket::Mode::UDP);
}

static void broadcastCreation(ecs::Entity entity, std::shared_ptr<network::Client> client) {
    for (auto& [destEntity, destClient]: entity.getRegistry()->getEntities<std::shared_ptr<network::Client>>()) {
        if (destEntity == entity) {
            continue;
        }
        getInfo(entity, client, destEntity, destClient);
        sendInfo(entity, client, destEntity, destClient);
    }
}

void registerClientEntity(std::shared_ptr<network::Client> client, int id) {
    auto registry = ecs::RegistryManager::getInstance().getRegistry(id);

    ecs::RegistryManager::getInstance().bind(client, id);
    ecs::Entity entity = EntitySchematic::createTeamPlayer(registry, client->getID(), 50, 50, 22);

    registry->setComponent(entity, client);
    broadcastCreation(entity, client);
}

void unregisterClientEntity(std::shared_ptr<network::Client> client) {
    auto registry = ecs::RegistryManager::getInstance().get(client);
    if (registry.has_value()) {
        ecs::Entity entity(client->getID(), registry.value());
        registry.value()->removeEntity(entity);
    }
}
