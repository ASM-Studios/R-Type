#pragma once

#include "Clock.hpp"
#include "Entity.hpp"
#include "GameLogic.hpp"
#include "Registry.hpp"
#include "query/RawRequest.hpp"
#include "socket/Client.hpp"
#include "socket/Server.hpp"
#include <array>
#include <atomic>
#include <map>
#include <span>
#include <string_view>
#include <vector>

class Core {
    private:
        int _tps;
        int _tickTime;
        Clock _tpsClock;
        std::atomic<bool> _isRunning;
        std::string _hitboxes_config_file;

        void _exit(std::vector<std::string> args);
        void _info(std::vector<std::string> args);

        void _readStdin();
        void _loop();

        const std::map<std::string_view, void (Core::*)(std::vector<std::string> args)> _stdinMap = {
            {"/exit", &Core::_exit},
            {"/info", &Core::_info}};

    public:
        explicit Core();
        ~Core() = default;

        void init(const std::span<char *>& args);
        int run();
};
