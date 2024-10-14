#pragma once

#include "Header.hpp"
#include "query.hpp"
#include <array>
#include <cstring>

class NonTypedQuery;

template <typename Payload>
class TypedQuery {
    private:
        RequestType _requestType;
        Payload _payload;
        std::array<char, MAX_PAYLOAD_SIZE - sizeof(Payload)> _fill;

        friend class NonTypedQuery;

    public:
        explicit TypedQuery(RequestType requestType, Payload payload);
        explicit TypedQuery(NonTypedQuery query);
        ~TypedQuery() = default;

        operator NonTypedQuery();

        [[nodiscard]] RequestType getRequestType() const {
            return this->_requestType;
        }

        void setRequestType(RequestType requestType) {
            this->_requestType = requestType;
        }

        [[nodiscard]] Payload getPayload() const {
            return this->_payload;
        }

        void setPayload(Payload payload) {
            this->_payload(payload);
        }
};

#include "TypedQuery.inl"
