#pragma once

#include <iostream>

enum class RequestType {
    NOTHING = 0x0000,
    // Server to client
    UPDATE_PLAYER = 0x0001,
    UPDATE_TEAM_PLAYER = 0x0002,
    CREATE_ENTITY = 0x0003,
    UPDATE_ENTITY = 0x0004,
    DESTROY_ENTITY = 0x0005,
    // Client to server
    INPUT = 0x0010
};

std::ostream& operator<<(std::ostream& os, RequestType requestType);
