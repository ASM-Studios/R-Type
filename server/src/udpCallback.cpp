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

void handlePing(network::Client client, RawRequest request) {
    auto timestamp = std::chrono::system_clock::now().time_since_epoch();
    TypedQuery<decltype(timestamp)> typedQuery = request.getQuery();

    timestamp = timestamp - typedQuery.getPayload();
    network::socket::ServerManager::getInstance().getServer().send(client.getIP().to_string(), client.getPort(), RawRequest(request.getQuery()));
}

const std::map<RequestType, void (*)(network::Client client, RawRequest rawRequest)> requestAction = {
    {RequestType::INPUT, &handleInput},
    {RequestType::PING, &handlePing}};
