#include "query/Header.hpp"
#include <map>

static const std::map<RequestType, std::string_view> binding = {
    {RequestType::NOTHING, "NOTHING"},
    {RequestType::UPDATE_PLAYER, "UPDATE_PLAYER"},
    {RequestType::UPDATE_TEAM_PLAYER, "UPDATE_TEAM_PLAYER"},
    {RequestType::CREATE_ENTITY, "CREATE_ENTITY"},
    {RequestType::UPDATE_ENTITY, "UPDATE_ENTITY"},
    {RequestType::DESTROY_ENTITY, "DESTROY_ENTITY"},
    {RequestType::INPUT, "INPUT"},
    {RequestType::PING, "PING"}};

std::ostream& operator<<(std::ostream& os, RequestType requestType) {
    os << binding.at(requestType);
    return os;
}

std::string to_string(RequestType requestType) {
    return std::string(binding.at(requestType));
}
