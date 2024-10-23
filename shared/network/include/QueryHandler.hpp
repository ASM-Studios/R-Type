#pragma once

#include "Client.hpp"
#include "query/RawRequest.hpp"
#include <boost/asio/thread_pool.hpp>
#include <memory>
#include <mutex>
#include <queue>

namespace network {
    class QueryHandler {
        private:
            static std::unique_ptr<QueryHandler> _instance;
            static std::mutex _mutex;

            boost::asio::thread_pool _pool;
            std::queue<std::pair<Client, RawRequest>> _pendingQueries;

            QueryHandler();

        public:
            QueryHandler(const QueryHandler& other) = delete;
            ~QueryHandler();

            QueryHandler& operator=(const QueryHandler& other) = delete;

            static QueryHandler& getInstance();

            void addQuery(std::pair<Client, RawRequest> query);
    };
}
