#pragma once

#include "GameLogic.hpp"
#include <mutex>

class GameLogicManager {
    private:
        static std::unique_ptr<GameLogicManager> _instance;
        static std::mutex _mutex;

        GameLogic _gameLogic;

        explicit GameLogicManager();

    public:
        GameLogicManager(const GameLogicManager& other) = delete;
        GameLogicManager& operator=(const GameLogicManager& other) = delete;

        static GameLogicManager& getInstance();

        GameLogic& get();
};
