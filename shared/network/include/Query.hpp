#pragma once

#include <cstdint>

#if 0
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
#else
struct Query {
    char req;
};
#endif
