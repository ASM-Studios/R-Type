#include "EntityInfo.hpp"
#include "Entity.hpp"
#include "Registry.hpp"

namespace ecs::system {
    std::vector<ecs::Entity> PlayerSystem::getPlayers() {
        std::vector<ecs::Entity> players;
        for (auto entity: ecs::Registry::getEntities()) {
            if (entity.contains<component::PlayerComponent>()) {
                players.emplace_back(entity);
            }
        }
        return players;
    }
}
