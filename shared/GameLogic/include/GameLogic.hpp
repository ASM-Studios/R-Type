#pragma once

#include "Entity.hpp"
#include "Registry.hpp"
#include "Position.hpp"
#include "Input.hpp"
#include "Logger.hpp"
#include "RegistryManager.hpp"
#include "LastShot.hpp"
#include "EntitySchematic.hpp"
#include <algorithm>
#include <chrono>

constexpr auto SHOOT_COOLDOWN = 0.1F;
constexpr auto SPEED = 5;


class GameLogic {
    public:
        GameLogic();

        void updateTimed();

    private:
        float _timePerTick;

        void update();
        void updateBullets();
        void handleInput(const ecs::Entity& entity);
};


