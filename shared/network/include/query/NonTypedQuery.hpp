#pragma once

#include "Header.hpp"
#include "query.hpp"
#include <array>
#include <cstring>

template <typename Payload>
class TypedQuery;

class NonTypedQuery {
    private:
        Header _header;
        std::array<char, MAX_PAYLOAD_SIZE> _fill;

        template <typename T>
        friend class TypedQuery;

    public:
        explicit NonTypedQuery() :
            _header(RequestType::NOTHING),
            _fill() {
            std::fill(this->_fill.begin(), this->_fill.end(), 0);
        }
        template <typename Payload>
        explicit NonTypedQuery(TypedQuery<Payload> query);

        template <typename Payload>
        operator TypedQuery<Payload>();

        [[nodiscard]] Header getHeader() const {
            return this->_header;
        }

        void setHeader(Header header) {
            this->_header = header;
        }
} __attribute__((packed));

#include "NonTypedQuery.inl"
