#pragma once

#include "Client.hpp"
#include "NonTypedQuery.hpp"
#include <boost/asio/buffer.hpp>
#include <cstdint>
#include <map>

class RawRequest {
    private:
        NonTypedQuery _query;

    public:
        explicit RawRequest() = default;
        explicit RawRequest(NonTypedQuery query);
        ~RawRequest() = default;

        operator boost::asio::const_buffer();

        [[nodiscard]] NonTypedQuery getQuery() const;
        void setQuery(NonTypedQuery query);
};

extern const std::map<RequestType, void (*)(network::Client client, RawRequest rawRequest)> requestAction;
