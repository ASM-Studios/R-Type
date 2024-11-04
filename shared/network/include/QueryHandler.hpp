#pragma once

#include "query/RawRequest.hpp"
#include "socket/Client.hpp"
#include <boost/asio/thread_pool.hpp>
#include <memory>
#include <mutex>

namespace network {
    class QueryHandler {
        private:
            static std::unique_ptr<QueryHandler> _instance;
            static std::mutex _mutex;

            boost::asio::thread_pool _pool;
            std::mutex _poolMutex;

            std::mutex _callbackMutex;
            void executeUdp(std::pair<std::shared_ptr<network::Client>, RawRequest> request);
            void executeTcp(std::pair<std::shared_ptr<network::Client>, RawRequest> request);

            QueryHandler();

        public:
            QueryHandler(const QueryHandler& other) = delete;
            ~QueryHandler();

            QueryHandler& operator=(const QueryHandler& other) = delete;

            static QueryHandler& getInstance();

            void addUdpQuery(std::pair<std::shared_ptr<Client>, RawRequest> query);
            void addTcpQuery(std::pair<std::shared_ptr<Client>, RawRequest> query);
    };
}
