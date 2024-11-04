#pragma once

#include <iostream>

enum class RequestType {
    NOTHING = 0x0000,
    // Server to client
    UPDATE_PLAYER = 0x0001,      // UDP
    UPDATE_TEAM_PLAYER = 0x0002, // UDP
    CREATE_ENTITY = 0x0003,      // UDP
    UPDATE_ENTITY = 0x0004,      // UDP
    DESTROY_ENTITY = 0x0005,     // UDP
    // Client to server
    CONNECT = 0x0010,    // TCP
    DISCONNECT = 0x0011, // TCP
    INIT = 0x0011,       // TCP
    INPUT = 0x0012,      // UDP
    // Shared
    PING = 0x0020 // UDP
};

std::ostream& operator<<(std::ostream& os, RequestType requestType);
std::string to_string(RequestType requestType);
