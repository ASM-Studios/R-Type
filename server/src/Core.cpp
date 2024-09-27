#include "Core.hpp"
#include "../modules/game/include/EntityInfo.hpp"
#include "../modules/game/include/Player.hpp"
#include "../modules/game/include/Registry.hpp"
#include "../modules/network/include/QueryHandler.hpp"
#include "../modules/network/include/socket/Server.hpp"
#include "Config.hpp"
#include "Logger.hpp"
#include <boost/algorithm/string/split.hpp>
#include <cstdlib>
#include <iostream>

void Core::_stop() {
    this->_isRunning = false;
}

void Core::_waitTPS() {
    double elapsed = this->_tpsClock.get();
    if (elapsed < this->_tickTime) {
        int time = static_cast<int>((this->_tickTime - elapsed) * 1000);
        std::this_thread::sleep_for(std::chrono::microseconds(time));
    }
    this->_tpsClock.reset();
}

void Core::_readStdin() {
    while (this->_isRunning) {
        std::string line;
        std::getline(std::cin, line);
        line = line.substr(0, line.find('\n'));
        for (auto command: _stdinMap) {
            if (command.first == line) {
                (*this.*command.second)();
            }
        }
    }
}

void Core::_loop(network::socket::udp::Server& server) {
    if (server.availableRequest()) {
        auto query = server.recv<Query>();
        network::Client client = query.first;
        if (!getEntityByClient(client).has_value()) {
            ecs::Entity entity = ecs::Registry::createEntity<>();
            entity.setComponent<ecs::component::Player>(ecs::component::Player(query.first));
        }
        network::QueryHandler::getInstance().addQuery(query);
    }
    network::QueryHandler::getInstance().executeQueries();
    network::QueryHandler::getInstance().checkWorkers();
}

Core::Core() :
    _isRunning(true) {}

void Core::init(const std::span<char *>& args [[maybe_unused]]) {
    Config& config = Config::getInstance(DEFAULT_CONFIG);

    this->_tps = std::atoi(config.get("tps").value_or("20").c_str());
    _tickTime = 1000 / _tps;
    this->_port = std::atoi(config.get("port").value_or("8080").c_str());
    this->_hitboxes_config_file = config.get("hitboxes_config_file").value_or("");
    Logger::log(LogLevel::INFO, std::format("Server running on port {0}", this->_port));
}

int Core::run() {
    network::QueryHandler& handler = network::QueryHandler::getInstance();
    network::socket::udp::Server server(_port);
    std::thread stdinThread(&Core::_readStdin, this);

    while (this->_isRunning) {
        this->_loop(server);
        this->_waitTPS();
    }
    stdinThread.join();
    return 0;
}
