#pragma once

#include "Logger.hpp"
#include "NonTypedQuery.hpp"
#include "TypedQuery.hpp"
#include <cstring>

template <typename Payload>
NonTypedQuery::NonTypedQuery(TypedQuery<Payload> query) :
    _requestType(query.getRequestType()),
    _fill() {
    std::fill(this->_fill.begin(), this->_fill.end(), 0);
    auto payload = query.getPayload();
    std::memcpy(&this->_fill, &payload, sizeof(Payload));
}

template <typename Payload>
NonTypedQuery::operator TypedQuery<Payload>() {
    return TypedQuery<Payload>(*this);
}
