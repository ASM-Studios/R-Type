#pragma once

#include "Header.hpp"
#include "query.hpp"
#include <array>
#include <cstring>

template <typename Payload>
class TypedQuery;

class NonTypedQuery {
    private:
        RequestType _requestType;
        std::array<char, MAX_PAYLOAD_SIZE> _fill;

        template <typename T>
        friend class TypedQuery;

    public:
        explicit NonTypedQuery() :
            _requestType(RequestType::NOTHING),
            _fill() {
            std::fill(this->_fill.begin(), this->_fill.end(), 0);
        }
        template <typename Payload>
        explicit NonTypedQuery(TypedQuery<Payload> query);

        template <typename Payload>
        operator TypedQuery<Payload>();

        [[nodiscard]] RequestType getRequestType() const {
            return this->_requestType;
        }

        void setRequestType(RequestType requestType) {
            this->_requestType = requestType;
        }
};

#include "NonTypedQuery.inl"
