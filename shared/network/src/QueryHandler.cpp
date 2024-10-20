#include "QueryHandler.hpp"
#include "Logger.hpp"
#include <format>
#include <mutex>
#include <stdexcept>

#define NO_WORKER       10

void execute(std::pair<network::Client, RawRequest> request) {
    try {
        auto callback = requestAction.at(request.second.getQuery().getRequestType());
        callback(request.first, request.second);
    } catch (std::out_of_range &e) {
        Logger::log(LogLevel::ERR, std::format("Callback not found"));
    }
}

namespace network {
    QueryHandler::QueryHandler() :
        _pool(boost::asio::thread_pool(NO_WORKER)) {}

    QueryHandler::~QueryHandler() {
        this->_pool.join();
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
        boost::asio::post(this->_pool, [query](){
            execute(query);
        });
    }

    void QueryHandler::executeQueries() {
        const std::size_t size = this->_pendingQueries.size();
        for (std::size_t i = 0; i < size; i++) {
            auto query = this->_pendingQueries.front();
            this->_pendingQueries.pop();
            this->executeQuery(query);
        }
    }

    std::unique_ptr<QueryHandler> QueryHandler::_instance(nullptr);
    std::mutex QueryHandler::_mutex;
}
