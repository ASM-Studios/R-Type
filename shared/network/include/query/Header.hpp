#pragma once

enum RequestType {
    NOTHING = 0x0000,
    // Server to client
    UPDATE_PLAYER = 0x0001,
    UPDATE_ENTITY = 0x0002,
    DESTROY_ENTITY = 0x0003
    // Client to server
};

struct Header {
    RequestType requestType;
};
