#include "RegistryManager.hpp"
#include "Singleton.hpp"
#include "query/Header.hpp"
#include "query/Payloads.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"
#include "socket/NRegistry.hpp"
#include <map>

void registerClientEntity(std::shared_ptr<network::Client> client, int id);

static void handleConnect(std::shared_ptr<network::Client> client, RawRequest rawRequest) {
    TypedQuery<Connect> tq = rawRequest.getQuery();
    registerClientEntity(client, tq.getPayload().id);
    ecs::RegistryManager::getInstance().bind(client, tq.getPayload().id);
}

static void handleDisconnect(std::shared_ptr<network::Client> client, RawRequest rawRequest [[maybe_unused]]) {
    ecs::RegistryManager::getInstance().unbind(client);
}

static void handleInit(std::shared_ptr<network::Client> client, RawRequest rawRequest) {
}

const std::map<RequestType, void (*)(std::shared_ptr<network::Client> client, RawRequest rawRequest)> tcpRequestAction = {
    {RequestType::CONNECT, &handleConnect},
    {RequestType::DISCONNECT, &handleDisconnect},
    {RequestType::INIT, &handleInit}};
