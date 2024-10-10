#include "query/RawRequest.hpp"
#include "query/NonTypedQuery.hpp"
#include <boost/crc.hpp>

RawRequest::RawRequest(NonTypedQuery query) :
    _query(query) {}

RawRequest::operator boost::asio::const_buffer() {
    return boost::asio::buffer(this, sizeof(RawRequest));
}

NonTypedQuery RawRequest::getQuery() const {
    return this->_query;
}

void RawRequest::setQuery(NonTypedQuery query) {
    this->_query = query;
}
