#include "QueryHandler.hpp"
#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std::chrono_literals;

namespace network {
    QueryHandler::~QueryHandler() {
        if (this->_workers.size() > 0) {
            std::cout << "Waiting for " << this->_workers.size() << " worker(s)" << std::endl;
        }
        for (auto &worker : this->_workers) {
            worker->join();
        }
    }

    std::shared_ptr<QueryHandler> QueryHandler::getInstance() {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_instance == nullptr) {
            _instance = std::shared_ptr<QueryHandler>(new QueryHandler());
        }
        return _instance;
    }

    void QueryHandler::addQuery(Query &query) {
        this->_pendingQueries.push(query);
    }

    void QueryHandler::executeQuery(Query query) {
        auto worker = std::make_shared<Worker>(query);
        this->_workers.emplace_back(worker);
    }

    void QueryHandler::executeQueries() {
        const std::size_t size = this->_pendingQueries.size();
        for (std::size_t i = 0; i < size; i++) {
            auto query = this->_pendingQueries.front();
            this->_pendingQueries.pop();
            this->executeQuery(query);
        }
    }

    void QueryHandler::checkWorkers() {
        for (auto &worker : this->_workers) {
            if (worker->isReady()) {
                std::cout << "end of worker" << std::endl;
                worker->join();
                auto it = std::find(this->_workers.begin(), this->_workers.end(), worker);
                this->_workers.erase(it);
            }
        }
    }

    std::shared_ptr<QueryHandler> QueryHandler::_instance(nullptr);
    std::mutex QueryHandler::_mutex;
}
