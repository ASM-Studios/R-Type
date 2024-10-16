#pragma once

#include "Client.hpp"
#include "Clock.hpp"
#include "Entity.hpp"
#include "GameLogic.hpp"
#include "Registry.hpp"
#include "query/RawRequest.hpp"
#include "socket/Server.hpp"
#include <array>
#include <atomic>
#include <map>
#include <span>
#include <string_view>

class Core {
    private:
        int _tps;
        int _tickTime;
        Clock _tpsClock;
        std::atomic<bool> _isRunning;
        int _port;
        std::string _hitboxes_config_file;
        GameLogic _gameLogic;

        void _stop();
        void _readStdin();
        void _loop();

        const std::map<std::string_view, void (Core::*)()> _stdinMap = {
            {"/exit", &Core::_stop}};

    public:
        explicit Core();
        ~Core() = default;

        void init(const std::span<char *>& args);
        int run();
};
