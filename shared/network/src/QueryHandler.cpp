#include "QueryHandler.hpp"
#include "Logger.hpp"
#include <format>
#include <mutex>

namespace network {
    QueryHandler::~QueryHandler() {
        if (this->_workers.size() > 0) {
            Logger::log(LogLevel::INFO, std::format("Waiting for {0} workers", this->_workers.size()));
        }
        for (auto& worker: this->_workers) {
            worker->join();
        }
    }

    QueryHandler& QueryHandler::getInstance() {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_instance == nullptr) {
            _instance = std::unique_ptr<QueryHandler>(new QueryHandler());
        }
        return *_instance;
    }

    void QueryHandler::addQuery(std::pair<Client, RawRequest> query) {
        this->_pendingQueries.push(query);
    }

    void QueryHandler::executeQuery(std::pair<Client, RawRequest> query) {
        auto worker = std::make_shared<Worker>(query);
        this->_workers.push_back(worker);
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
        for (auto it = this->_workers.begin(); it != this->_workers.end();) {
            auto worker = *it;
            if (worker->isReady()) {
                worker->join();
                it = this->_workers.erase(it);
            } else {
                it++;
            }
        }
    }

    std::unique_ptr<QueryHandler> QueryHandler::_instance(nullptr);
    std::mutex QueryHandler::_mutex;
}
