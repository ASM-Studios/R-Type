#include "Input.hpp"
#include "socket/Client.hpp"
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
#include "socket/NetworkManager.hpp"
#include <boost/asio/buffer.hpp>
#include <csignal>
#include <cstdio>
#include <iomanip>
#include <iostream>

ecs::Entity registerClientEntity(std::shared_ptr<network::Client> client);

void handleInput(std::shared_ptr<network::Client> client, RawRequest request) {
    auto entity = registerClientEntity(client);
    TypedQuery<ecs::component::Input> query = request.getQuery();
    ecs::RegistryManager::getInstance().getRegistry().setComponent(entity, query.getPayload());
}

void handlePing(std::shared_ptr<network::Client> client, RawRequest request) {
    auto timestamp = std::chrono::system_clock::now().time_since_epoch();
    TypedQuery<decltype(timestamp)> typedQuery = request.getQuery();

    timestamp = timestamp - typedQuery.getPayload();
    network::socket::NetworkManager::getInstance().send(client, RawRequest(request.getQuery()), network::socket::Mode::UDP);
}

const std::map<RequestType, void (*)(std::shared_ptr<network::Client> client, RawRequest rawRequest)> udpRequestAction = {
    {RequestType::INPUT, &handleInput},
    {RequestType::PING, &handlePing}};
