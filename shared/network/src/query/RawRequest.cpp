#include "query/RawRequest.hpp"
#include "Singleton.hpp"
#include "query/NonTypedQuery.hpp"
#include <boost/crc.hpp>
#include <boost/uuid/uuid.hpp>

RawRequest::RawRequest(NonTypedQuery query) :
    _uuid(),
    _query(query) {

    Singleton<boost::uuids::uuid>::getInstance().lock();
    this->_uuid = Singleton<boost::uuids::uuid>::getInstance().get(); // NOLINT
    Singleton<boost::uuids::uuid>::getInstance().unlock();
}

RawRequest::operator boost::asio::const_buffer() {
    return boost::asio::buffer(this, sizeof(RawRequest));
}

NonTypedQuery RawRequest::getQuery() const {
    return this->_query;
}

void RawRequest::setQuery(NonTypedQuery query) {
    this->_query = query;
}

boost::uuids::uuid RawRequest::getUuid() const {
    return this->_uuid;
}
