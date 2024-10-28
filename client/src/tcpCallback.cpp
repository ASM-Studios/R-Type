#include "query/Header.hpp"
#include "query/RawRequest.hpp"
#include "socket/Client.hpp"
#include <map>

const std::map<RequestType, void (*)(std::shared_ptr<network::Client> client, RawRequest rawRequest)> tcpRequestAction = {
};
