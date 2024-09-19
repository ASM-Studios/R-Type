#include "QueryHandler.hpp"
#include "Logging.hpp"
#include <algorithm>
#include <mutex>

namespace network {
    QueryHandler::~QueryHandler() {
        if (this->_workers.size() > 0) {
            Logging::info(std::string("Waiting for ") + std::to_string(this->_workers.size()) + " worker(s)");
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
        for (auto it = this->_workers.begin(); it != this->_workers.end();) {
            auto worker = *it;
            if (worker->isReady()) {
                Logging::info("End of worker");
                worker->join();
                it = this->_workers.erase(it);
            } else {
                it++;
            }
        }
    }

    std::shared_ptr<QueryHandler> QueryHandler::_instance(nullptr);
    std::mutex QueryHandler::_mutex;
}
