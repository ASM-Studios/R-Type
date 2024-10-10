#pragma once

#include "Entity.hpp"
#include "Registry.hpp"
#include "Position.hpp"
#include "Input.hpp"
#include "Logger.hpp"
#include "RegistryManager.hpp"
#include "LastShot.hpp"
#include "EntitySchematic.hpp"
#include "Collison.hpp"
#include "Animation.hpp"
#include <algorithm>
#include <chrono>
#include <thread>

constexpr auto SHOOT_COOLDOWN = 0.2F;


class GameLogic {
    public:
        GameLogic();

        void updateTimed();
    private:
        float _timePerTick;

        void update();
        void updateAnimation(const ecs::Entity& entity);
};


