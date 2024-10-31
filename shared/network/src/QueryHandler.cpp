#include "QueryHandler.hpp"
#include "Logger.hpp"
#include "query/RawRequest.hpp"
#include <format>
#include <mutex>
#include <stdexcept>

#define NO_WORKER 10

namespace network {
    void QueryHandler::executeUdp(std::pair<std::shared_ptr<network::Client>, RawRequest> request) {
        try {
            this->_callbackMutex.lock();
            auto callback = udpRequestAction.at(request.second.getQuery().getRequestType());
            this->_callbackMutex.unlock();
            callback(request.first, request.second);
        } catch (const std::out_of_range& e) {
            Logger::log(LogLevel::ERR, std::format("Callback not found: {}", to_string(request.second.getQuery().getRequestType())));
        }
    }

    void QueryHandler::executeTcp(std::pair<std::shared_ptr<Client>, RawRequest> request) {
        try {
            this->_callbackMutex.lock();
            auto callback = tcpRequestAction.at(request.second.getQuery().getRequestType());
            this->_callbackMutex.unlock();
            callback(request.first, request.second);
        } catch (const std::out_of_range& e) {
            Logger::log(LogLevel::ERR, std::format("Callback not found: {}", to_string(request.second.getQuery().getRequestType())));
        }
    }

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

    void QueryHandler::addUdpQuery(std::pair<std::shared_ptr<Client>, RawRequest> query) {
        this->executeUdp(query);
        /*std::lock_guard<std::mutex> lock(this->_poolMutex);
        boost::asio::post(this->_pool, [this, query]() {
            this->executeUdp(query);
        });*/
    }

    void QueryHandler::addTcpQuery(std::pair<std::shared_ptr<Client>, RawRequest> query) {
        this->executeTcp(query);
        /*std::lock_guard<std::mutex> lock(this->_poolMutex);
        boost::asio::post(this->_pool, [this, query]() {
            this->executeTcp(query);
        });*/
    }

    std::unique_ptr<QueryHandler> QueryHandler::_instance(nullptr);
    std::mutex QueryHandler::_mutex;
}
