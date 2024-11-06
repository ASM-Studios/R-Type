#include "GameLogicManager.hpp"
#include "GameLogicMode.hpp"

GameLogicManager::GameLogicManager() :
    _gameLogic(GameLogic(GAMELOGICMODE)) {}

GameLogicManager& GameLogicManager::getInstance() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_instance == nullptr) {
        _instance = std::unique_ptr<GameLogicManager>(new GameLogicManager());
    }
    return *_instance;
}

GameLogic& GameLogicManager::get() {
    return this->_gameLogic;
}

std::unique_ptr<GameLogicManager> GameLogicManager::_instance(nullptr);
std::mutex GameLogicManager::_mutex;
