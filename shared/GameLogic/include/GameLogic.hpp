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
#include "ProceduralGenerator.hpp"

constexpr auto SHOOT_COOLDOWN = 0.2F;
class ProceduralGenerator;
class GameLogic {
    public:
        GameLogic(GameLogicMode mode);

        void updateTimed();

    private:
        GameLogicMode _mode;
        float _timePerTick;
        float _totalTime;
        Clock _clock;

        std::unique_ptr<ProceduralGenerator> _proceduralGenerator;

        std::vector<ecs::factory::LevelFactory> _levels;

        void update();

        void client(const ecs::Entity& entity);
        void sendInput(const ecs::Entity& entity);
        void updateAnimation(const ecs::Entity& entity);

        void server(const ecs::Entity& entity);
        void sendPlayerPosition(const ecs::Entity& entity);
        void sendTeamPosition(const ecs::Entity& entity);
};
