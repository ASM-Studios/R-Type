#include "Player.hpp"

std::optional<ecs::Entity> getEntityByClient(const network::Client& client) {
    for (auto entity: ecs::Registry::getEntities()) {
        if (entity.contains<ecs::component::Player>()) {
            if (entity.getComponent<ecs::component::Player>()->get().client == client) {
                return entity;
            }
        }
    }
    return std::nullopt;
}
