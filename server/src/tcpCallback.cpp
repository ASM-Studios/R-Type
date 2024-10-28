#include "Singleton.hpp"
#include "query/Header.hpp"
#include "query/Payloads.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"
#include "socket/NRegistry.hpp"
#include <map>

static void handleInit(std::shared_ptr<network::Client> client, RawRequest rawRequest) {
    Singleton<network::Registry>::getInstance().get().registerClient(client, rawRequest.getUuid());
}

const std::map<RequestType, void (*)(std::shared_ptr<network::Client> client, RawRequest rawRequest)> tcpRequestAction = {
    {RequestType::INIT, &handleInit}};
