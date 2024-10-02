#pragma once

#include <cstdint>

struct Header {
    uint16_t requestType;
};

struct Empty {};

template <typename Payload = Empty>
struct Query {
    Header header;
    static constexpr uint32_t payloadSize = sizeof(Payload);
    Payload payload;
};

template <typename Payload = Empty>
struct Request {
    uint32_t checksum;
    Query<Payload> query;
};

enum StatusCode {
    OK,
    KO
};

extern "C" {
    struct CreateLobbyRequest {
        char username[64];
    };

    struct CreateLobbyResponse {
        uint32_t lobbyId;
    };

    struct JoinLobbyRequest {
        char username[64];
        uint32_t lobbyId;
    };
}
