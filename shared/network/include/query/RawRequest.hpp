#pragma once

#include "socket/Client.hpp"
#include "NonTypedQuery.hpp"
#include <boost/asio/buffer.hpp>
#include <cstdint>
#include <map>
#include <boost/uuid/uuid.hpp>

class RawRequest {
    private:
        boost::uuids::uuid _uuid;
        NonTypedQuery _query;

    public:
        explicit RawRequest() = default;
        explicit RawRequest(NonTypedQuery query);
        ~RawRequest() = default;

        operator boost::asio::const_buffer();

        [[nodiscard]] NonTypedQuery getQuery() const;
        void setQuery(NonTypedQuery query);

        [[nodiscard]] boost::uuids::uuid getUuid() const;
};

extern const std::map<RequestType, void (*)(std::shared_ptr<network::Client> client, RawRequest rawRequest)> udpRequestAction;
extern const std::map<RequestType, void (*)(std::shared_ptr<network::Client> client, RawRequest rawRequest)> tcpRequestAction;
