#include "Core.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "Logger.hpp"
#include "QueryHandler.hpp"
#include "ScopeDuration.hpp"
#include "TextureLoader.hpp"
#include "query/RawRequest.hpp"
#include "socket/Server.hpp"
#include <cstdlib>
#include <iostream>

void Core::_initTextures() {
    TextureLoader::getInstance().loadFile("assets/textures_config.cfg");
    TextureLoader::getInstance().loadTextures("backgrounds", TextureLoader::Type::BACKGROUND);
    TextureLoader::getInstance().loadTextures("bullets", TextureLoader::Type::BULLET);
    TextureLoader::getInstance().loadTextures("enemies", TextureLoader::Type::ENEMY);
    TextureLoader::getInstance().loadTextures("ships", TextureLoader::Type::SHIP);
    Logger::log(LogLevel::INFO, std::format("{0} textures have been loaded", TextureLoader::getInstance().getNoTexture()));
}

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

void Core::_loop(network::socket::udp::Server& server) {
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
    _isRunning(true),
    _port(8080) {}

void Core::init(const std::span<char *>& args [[maybe_unused]]) {
    Config& config = Config::getInstance("server/config.json");

    this->_tps = std::atoi(config.get("tps").value_or("20").c_str());
    _tickTime = 1000 / _tps;
    this->_port = std::atoi(config.get("port").value_or("8080").c_str());
    this->_hitboxes_config_file = config.get("hitboxes_config_file").value_or("");
    Logger::log(LogLevel::INFO, std::format("Server running on port {0}", this->_port));
    this->_initTextures();
}

int Core::run() {
    network::QueryHandler& handler = network::QueryHandler::getInstance();
    network::socket::udp::Server server(_port);
    std::thread stdinThread(&Core::_readStdin, this);

    while (this->_isRunning) {
        ScopeDuration duration(this->_tickTime);
        this->_loop(server);
    }
    stdinThread.join();
    return 0;
}
