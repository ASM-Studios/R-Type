#include "Core.hpp"
#include "../modules/network/include/QueryHandler.hpp"
#include "../modules/network/include/socket/Server.hpp"
#include "Logging.hpp"

void Core::_waitTPS() {
    double elapsed = this->_tpsClock.get();
    if (elapsed < this->_tickTime) {
        usleep(static_cast<int>((this->_tickTime - elapsed) * 1000));
    }
    this->_tpsClock.reset();
}

Core::Core() :
    _tps(20),
    _tickTime(1000 / _tps),
    _isRunning(true) {}

void Core::init(const std::span<char *>& args [[maybe_unused]]) {
}

int Core::run() {
    network::socket::udp::Server server(8080);

    while (this->_isRunning) {
        if (server.availableRequest()) {
            Logging::info("New request");
            auto query = server.recv<Query>();
            network::QueryHandler::getInstance()->addQuery(query);
        }
        network::QueryHandler::getInstance()->executeQueries();
        network::QueryHandler::getInstance()->checkWorkers();
        this->_waitTPS();
    }
    return 0;
}
