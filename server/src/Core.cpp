#include "Core.hpp"
#include "../modules/network/include/QueryHandler.hpp"
#include "../modules/network/include/socket/Server.hpp"
#include "Config.hpp"
#include "Logging.hpp"
#include <cstdlib>

void Core::_waitTPS() {
    double elapsed = this->_tpsClock.get();
    if (elapsed < this->_tickTime) {
        int time = static_cast<int>((this->_tickTime - elapsed) * 1000);
        std::this_thread::sleep_for(std::chrono::microseconds(time));
    }
    this->_tpsClock.reset();
}

Core::Core() :
    _isRunning(true) {}

void Core::init(const std::span<char *>& args [[maybe_unused]]) {
    Config& config = Config::getInstance(DEFAULT_CONFIG);

    this->_tps = std::atoi(config.get("tps").value_or("20").c_str());
    _tickTime = 1000 / _tps;
    this->_port = std::atoi(config.get("port").value_or("8080").c_str());
    this->_hitboxes_config_file = config.get("hitboxes_config_file").value_or("");
    Logging::info(std::format("Server running on port {0}", this->_port));
}

int Core::run() {
    network::socket::udp::Server server(_port);

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
