#pragma once

#include "Client.hpp"
#include "NonTypedQuery.hpp"
#include <boost/asio/buffer.hpp>
#include <cstdint>
#include <map>

class RawRequest {
    private:
        uint32_t _checksum;
        NonTypedQuery _query;

        void _reloadChecksum();

    public:
        explicit RawRequest() :
            _checksum(0) {}
        explicit RawRequest(NonTypedQuery query);
        ~RawRequest() = default;

        operator boost::asio::const_buffer();

        [[nodiscard]] NonTypedQuery getQuery() const;
        void setQuery(NonTypedQuery query);

        [[nodiscard]] bool verifChecksum() const;
} __attribute__((packed));

extern const std::map<RequestType, void (*)(network::Client client, RawRequest rawRequest)> requestAction;
