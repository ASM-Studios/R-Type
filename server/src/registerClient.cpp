#include "socket/Client.hpp"
#include "Entity.hpp"
#include "EntitySchematic.hpp"
#include "Logger.hpp"
#include "RegistryManager.hpp"
#include "Tags.hpp"
#include "query/Header.hpp"
#include "query/Payloads.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"
#include "socket/NetworkManager.hpp"
#include <boost/asio/buffer.hpp>
#include <cstdio>
#include <memory>

static void getInfo(ecs::Entity entity, std::shared_ptr<network::Client> client, ecs::Entity destEntity, std::shared_ptr<network::Client> destClient) {
    CreateEntity payload{destEntity.getID(), 23, ecs::RegistryManager::getInstance().getRegistry().getComponent<ecs::component::Position>(destEntity), ecs::RegistryManager::getInstance().getRegistry().getComponent<ecs::component::Tags>(destEntity)};
    TypedQuery typedQuery(RequestType::CREATE_ENTITY, payload);
    network::socket::NetworkManager::getInstance().send(client, RawRequest(typedQuery), network::socket::Mode::UDP);
}

static void sendInfo(ecs::Entity entity, std::shared_ptr<network::Client> client, ecs::Entity destEntity, std::shared_ptr<network::Client> destClient) {
    CreateEntity payload{entity.getID(), 23, ecs::RegistryManager::getInstance().getRegistry().getComponent<ecs::component::Position>(entity), ecs::RegistryManager::getInstance().getRegistry().getComponent<ecs::component::Tags>(entity)};
    TypedQuery typedQuery(RequestType::CREATE_ENTITY, payload);
    network::socket::NetworkManager::getInstance().send(destClient, RawRequest(typedQuery), network::socket::Mode::UDP);
}

static void broadcastCreation(ecs::Entity entity, std::shared_ptr<network::Client> client) {
    for (auto& [destEntity, destClient]: ecs::RegistryManager::getInstance().getRegistry().getEntities<std::shared_ptr<network::Client>>()) {
        if (destEntity == entity) {
            continue;
        }
        getInfo(entity, client, destEntity, destClient);
        sendInfo(entity, client, destEntity, destClient);
    }
}

ecs::Entity registerClientEntity(std::shared_ptr<network::Client> client) {
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    for (const auto& entity: registry.getEntities<std::shared_ptr<network::Client>>()) {
        if (entity.second == client) {
            return entity.first;
        }
    }
    Logger::log(LogLevel::INFO, std::format("{}", client->getID()));
    ecs::Entity entity = EntitySchematic::createTeamPlayer(client->getID(), 50, 50, 22);
    registry.setComponent(entity, client);
    broadcastCreation(entity, client);
    return entity;
}
