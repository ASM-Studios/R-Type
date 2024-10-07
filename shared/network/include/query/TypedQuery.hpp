#pragma once

#include "Header.hpp"
#include "query.hpp"
#include <array>
#include <cstring>

class NonTypedQuery;

template <typename Payload>
class TypedQuery {
private:
    Header _header;
    Payload _payload;
    std::array<char, MAX_PAYLOAD_SIZE - sizeof(Payload)> _fill;

    friend class NonTypedQuery;

public:
    explicit TypedQuery(Header header, Payload payload);
    explicit TypedQuery(NonTypedQuery query);
    ~TypedQuery() = default;

    operator NonTypedQuery();

    [[nodiscard]] Header getHeader() const {
        return this->_header;
    }

    void setHeader(Header header) {
        this->_header = header;
    }

    [[nodiscard]] Payload getPayload() const {
        return this->_payload;
    }

    void setPayload(Payload payload) {
        this->_payload(payload);
    }
} __attribute__((packed));

#include "TypedQuery.inl"
