#pragma once

#include <iostream>

enum class GameLogicMode {
    SERVER,
    CLIENT,
    BOTH
};

static inline std::ostream& operator<<(std::ostream& os, GameLogicMode mode) {
    switch (mode) {
        case GameLogicMode::SERVER:
            os << "SERVER";
            break;
        case GameLogicMode::CLIENT:
            os << "CLIENT";
            break;
        case GameLogicMode::BOTH:
            os << "BOTH";
            break;
    }
    return os;
}

static inline bool isPureServer(GameLogicMode mode) {
    return mode == GameLogicMode::SERVER;
}

static inline bool isServer(GameLogicMode mode) {
    return mode == GameLogicMode::SERVER || mode == GameLogicMode::BOTH;
}

static inline bool isPureClient(GameLogicMode mode) {
    return mode == GameLogicMode::CLIENT;
}

static inline bool isClient(GameLogicMode mode) {
    return mode == GameLogicMode::CLIENT || mode == GameLogicMode::BOTH;
}
