#pragma once

#include <iostream>

enum class GameLogicMode {
    SERVER,
    CLIENT,
    RUNNERG
};

extern const GameLogicMode GAMELOGICMODE;

static inline std::ostream& operator<<(std::ostream& os, GameLogicMode mode) {
    switch (mode) {
        case GameLogicMode::SERVER:
            os << "SERVER";
            break;
        case GameLogicMode::CLIENT:
            os << "CLIENT";
            break;
        case GameLogicMode::RUNNERG:
            os << "RUNNERG";
            break;
    }
    return os;
}

static inline bool isPureServer(GameLogicMode mode) {
    return mode == GameLogicMode::SERVER;
}

static inline bool isPureClient(GameLogicMode mode) {
    return mode == GameLogicMode::CLIENT;
}

static inline bool isPureRunner(GameLogicMode mode) {
    return mode == GameLogicMode::RUNNERG;
}