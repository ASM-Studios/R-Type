#pragma once

#include "Animation.hpp"
#include "Clock.hpp"
#include "Collision.hpp"
#include "Entity.hpp"
#include "EntitySchematic.hpp"
#include "Factories/LevelFactory.hpp"
#include "Input.hpp"
#include "LastShot.hpp"
#include "Logger.hpp"
#include "Position.hpp"
#include "Registry.hpp"
#include "RegistryManager.hpp"
#include <algorithm>
#include <chrono>
#include <thread>

constexpr auto SHOOT_COOLDOWN = 0.2F;

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
        Clock _clock;

        void update();

        void client(const ecs::Entity& entity);
        void sendInput(const ecs::Entity& entity);
        void updateAnimation(const ecs::Entity& entity);

        void server(const ecs::Entity& entity);
        void sendPlayerPosition(const ecs::Entity& entity);
        void sendTeamPosition(const ecs::Entity& entity);
};
