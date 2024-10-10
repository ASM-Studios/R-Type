#pragma once

#include "TypedQuery.hpp"
#include "NonTypedQuery.hpp"
#include <cstring>

template <typename Payload>
TypedQuery<Payload>::TypedQuery(RequestType requestType, Payload payload) :
    _requestType(requestType),
    _payload(),
    _fill({0}) {
    this->_payload = payload;
}

template <typename Payload>
TypedQuery<Payload>::TypedQuery(NonTypedQuery query) :
    _requestType(query.getRequestType()),
    _payload(),
    _fill({0}) {
    auto fill = query._fill;
    std::memcpy(&this->_payload, &fill, sizeof(Payload));
}

template <typename Payload>
TypedQuery<Payload>::operator NonTypedQuery() {
    return NonTypedQuery(*this);
}
