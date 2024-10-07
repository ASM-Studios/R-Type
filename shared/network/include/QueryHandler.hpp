#pragma once

#include "Client.hpp"
#include "Query.hpp"
#include "Worker.hpp"
#include <memory>
#include <mutex>
#include <queue>

namespace network {
    class QueryHandler {
    private:
        static std::unique_ptr<QueryHandler> _instance;
        static std::mutex _mutex;

        std::queue<std::pair<Client, Query>> _pendingQueries;
        std::vector<std::shared_ptr<Worker>> _workers;

        QueryHandler() = default;

    public:
        QueryHandler(const QueryHandler& other) = delete;
        ~QueryHandler();

        QueryHandler& operator=(const QueryHandler& other) = delete;

        static QueryHandler& getInstance();

        void addQuery(std::pair<Client, Query> query);
        void executeQuery(std::pair<Client, Query> query);
        void executeQueries();
        void checkWorkers();
    };
}