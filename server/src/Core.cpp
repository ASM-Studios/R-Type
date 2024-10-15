#include "Core.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "GameLogicMode.hpp"
#include "Logger.hpp"
#include "QueryHandler.hpp"
#include "ScopeDuration.hpp"
#include "TextureLoader.hpp"
#include "query/RawRequest.hpp"
#include "socket/Server.hpp"
#include "socket/ServerManager.hpp"
#include <cstdlib>
#include <iostream>

void Core::_stop() {
    this->_isRunning = false;
}

void Core::_readStdin() {
    while (this->_isRunning) {
        std::string line;
        std::getline(std::cin, line);
        line = line.substr(0, line.find('\n'));
        auto pair = this->_stdinMap.find(line);
        if (pair != this->_stdinMap.end()) {
            (*this.*pair->second)();
        } else {
            Logger::log(LogLevel::ERR, std::format("Command '{0}' does not exist", line));
        }
    }
}

void Core::_loop() {
    this->_gameLogic.updateTimed();
    auto& server = network::socket::udp::ServerManager::getInstance().getServer();
    if (server.availableRequest()) {
        auto query = server.recv<RawRequest>();
        network::Client client = query.first;
        network::QueryHandler::getInstance().addQuery(query);
    }
    network::QueryHandler::getInstance().executeQueries();
    network::QueryHandler::getInstance().checkWorkers();
}

Core::Core() :
    _tps(20),
    _tickTime(1000 / _tps),
    _gameLogic(GameLogicMode::SERVER),
    _isRunning(true),
    _port(8080) {}

void Core::init(const std::span<char *>& args [[maybe_unused]]) {
    Config& config = Config::getInstance("server/config.json");

    this->_tps = std::atoi(config.get("tps").value_or("20").c_str());
    _tickTime = 1000 / _tps;
    this->_port = std::atoi(config.get("port").value_or("8080").c_str());
    this->_hitboxes_config_file = config.get("hitboxes_config_file").value_or("");
    Logger::log(LogLevel::INFO, std::format("Server running on port {0}", this->_port));
}

int Core::run() {
    network::QueryHandler& handler = network::QueryHandler::getInstance();
    network::socket::udp::ServerManager::getInstance().init(this->_port);
    std::thread stdinThread(&Core::_readStdin, this);
    this->_gameLogic.start();

    while (this->_isRunning) {
        ScopeDuration duration(this->_tickTime);
        this->_loop();
    }
    stdinThread.join();
    return 0;
}
