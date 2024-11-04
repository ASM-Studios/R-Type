#include "Core.hpp"
#include "Entity.hpp"
#include "EntitySchematic.hpp"
#include "Input.hpp"
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
#include <csignal>
#include <cstdio>
#include <iomanip>
#include <iostream>

void handleInput(std::shared_ptr<network::Client> client, RawRequest request) {
    auto entity = ecs::getEntity(client);
    TypedQuery<ecs::component::Input> query = request.getQuery();
    auto registry = ecs::RegistryManager::getInstance().get(client);
    if (registry.has_value() && entity.has_value()) {
        registry.value()->setComponent(entity.value(), query.getPayload());
    }
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
