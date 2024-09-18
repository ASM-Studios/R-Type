#pragma once

#include "Query.hpp"
#include "Worker.hpp"
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

namespace network {
    class QueryHandler {
    private:
        static std::shared_ptr<QueryHandler> _instance;
        static std::mutex _mutex;

        std::queue<Query> _pendingQueries;
        std::vector<std::shared_ptr<Worker>> _workers;

        QueryHandler() = default;

    public:
        QueryHandler(const QueryHandler &other) = delete;
        ~QueryHandler();

        QueryHandler &operator=(const QueryHandler &other) = delete;

        static std::shared_ptr<QueryHandler> getInstance();

        void addQuery(Query &query);
        void executeQuery(Query query);
        void executeQueries();
        void checkWorkers();
    };
}
