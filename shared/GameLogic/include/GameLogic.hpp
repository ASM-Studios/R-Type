#pragma once

#include "Animation.hpp"
#include "Collision.hpp"
#include "Entity.hpp"
#include "EntitySchematic.hpp"
#include "Input.hpp"
#include "LastShot.hpp"
#include "Logger.hpp"
#include "Position.hpp"
#include "Registry.hpp"
#include "RegistryManager.hpp"
#include "LastShot.hpp"
#include "EntitySchematic.hpp"
#include "Collision.hpp"
#include "Animation.hpp"
#include "Factories/LevelFactory.hpp"
#include <algorithm>
#include <chrono>
#include <thread>

constexpr auto SHOOT_COOLDOWN = 0.2F;

enum GameLogicMode {
    SERVER,
    CLIENT
};

class GameLogic {
    public:
        GameLogic(GameLogicMode mode);

        void start();
        void stop();

        void updateTimed();

    private:
        GameLogicMode _mode;
        bool _isRunning;
        float _timePerTick;
        float _totalTime;
        void update();

        void client(ecs::Entity entity);
        void sendInput(ecs::Entity entity);
        void updateAnimation(const ecs::Entity& entity);

        void server(ecs::Entity entity);
        void sendPlayerPosition(ecs::Entity entity);
        void sendTeamPosition(ecs::Entity entity);
};
