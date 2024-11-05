#include "Core.hpp"
#include "Config.hpp"
#include "GameLogicManager.hpp"
#include "GameLogicMode.hpp"
#include "Logger.hpp"
#include "QueryHandler.hpp"
#include "RegistryManager.hpp"
#include "ScopeDuration.hpp"
#include "TextureLoader.hpp"
#include "query/RawRequest.hpp"
#include "socket/Client.hpp"
#include "socket/NRegistry.hpp"
#include "socket/NetworkManager.hpp"
#include "socket/Server.hpp"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cstdlib>
#include <iostream>

void Core::_exit(std::vector<std::string> args) {
    network::socket::NetworkManager::getInstance().getContext().stop();
    this->_isRunning = false;
}

void Core::_info(std::vector<std::string> args) {
    Singleton<network::Registry>::getInstance().lock();
    for (auto& [uuid, client]: Singleton<network::Registry>::getInstance().get().getClients()) {
        if (client->getUdpIP().has_value() && client->getUdpPort().has_value()) {
            Logger::log(LogLevel::INFO, std::format("{} {}", client->getUdpIP().value().to_string(), client->getUdpPort().value()));
        }
    }
    Singleton<network::Registry>::getInstance().unlock();
}

void Core::_readStdin() {
    while (this->_isRunning) {
        std::string line;
        std::getline(std::cin, line);
        if (line.length() == 0) {
            this->_exit(std::vector<std::string>());
            return;
        }
        line = line.substr(0, line.find('\n'));
        auto pair = this->_stdinMap.find(line);
        if (pair != this->_stdinMap.end()) {
            std::vector<std::string> args;
            boost::split(args, line, boost::is_any_of(" "));
            (*this.*pair->second)(args);
        } else {
            Logger::log(LogLevel::ERR, std::format("Command '{0}' does not exist", line));
        }
    }
}

void Core::_loop() {
    std::thread runServer([]() {
        network::socket::NetworkManager::getInstance().getContext().run();
    });

    while (this->_isRunning) {
        ScopeDuration duration(50);
        GameLogicManager::getInstance().get().updateTimed();
    }
    runServer.join();
}

Core::Core() :
    _tps(20), _tickTime(1000 / _tps),
    _isRunning(true) {}

void Core::init(const std::span<char *>& args [[maybe_unused]]) {
    Config& config = Config::getInstance("server/config.json");

    this->_tps = std::atoi(config.get("tps").value_or("20").c_str());
    _tickTime = 1000 / _tps;
    int udpPort = std::atoi(config.get("udp_port").value_or("8080").c_str());
    int tcpPort = std::atoi(config.get("tcp_port").value_or("8081").c_str());
    this->_hitboxes_config_file = config.get("hitboxes_config_file").value_or("");
    Logger::log(LogLevel::INFO, std::format("Server running on port {0}", udpPort));

    network::socket::NetworkManager::getInstance().init(udpPort, tcpPort);
}

int Core::run() {
    std::thread stdinThread(&Core::_readStdin, this);

    this->_loop();

    stdinThread.join();
    return 0;
}
