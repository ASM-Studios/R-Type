#pragma once

#include <cstdint>

struct Header {
    uint16_t requestType;
} __attribute__((packed));

#if 0
struct Empty {};

template <typename Payload = Empty>
struct Query {
    Header header;
    static constexpr uint32_t payloadSize = sizeof(Payload);
    Payload payload;
} __attribute__((packed));

template <typename Payload>
struct Request {
    uint32_t checksum;
    Query<Payload> query;
};
#else
struct Query {
    char req;
};
#endif
