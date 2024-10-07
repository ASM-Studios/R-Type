#pragma once

#include "socket/Server.hpp"
#include "Clock.hpp"
#include <array>
#include <atomic>
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

    void _initTextures();

    void _stop();
    void _waitTPS();
    void _readStdin();
    void _loop(network::socket::udp::Server& server);

    static constexpr std::array<std::pair<std::string_view, void (Core::*)()>, 1> _stdinMap = {{{"/exit", &Core::_stop}}};

public:
    explicit Core();
    ~Core() = default;

    void init(const std::span<char *>& args);
    int run();
};
