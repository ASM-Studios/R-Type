#include "query/RawRequest.hpp"
#include "query/NonTypedQuery.hpp"
#include <boost/crc.hpp>

void RawRequest::_reloadChecksum() {
    boost::crc_32_type result;
    result.process_bytes(&this->_query, sizeof(NonTypedQuery));
    this->_checksum = result.checksum();
}

RawRequest::RawRequest(NonTypedQuery query) :
    _checksum(0),
    _query(query) {
    this->_reloadChecksum();
}

RawRequest::operator boost::asio::const_buffer() {
    return boost::asio::buffer(this, sizeof(RawRequest));
}

NonTypedQuery RawRequest::getQuery() const {
    return this->_query;
}

void RawRequest::setQuery(NonTypedQuery query) {
    this->_query = query;
    this->_reloadChecksum();
}

bool RawRequest::verifChecksum() const {
    boost::crc_32_type result;
    result.process_bytes(&this->_query, sizeof(NonTypedQuery));
    return this->_checksum == result.checksum();
}
