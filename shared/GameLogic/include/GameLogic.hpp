#pragma once

#include "Entity.hpp"
#include "Registry.hpp"
#include "Position.hpp"
#include "Input.hpp"
#include "Logger.hpp"
#include "RegistryManager.hpp"

constexpr auto SPEED = 5;


class GameLogic {
    public:
        GameLogic() : _registry(ecs::RegistryManager::getInstance().getRegistry()) {}

        void update();

    private:
        ecs::Registry &_registry;

        void updateBullets();
        void createBullet(const ecs::Entity& shooter);
        void handleInput(const ecs::Entity& entity);
};


