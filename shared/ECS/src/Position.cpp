#include "Position.hpp"
#include "Registry.hpp"

namespace ecs::system {
    void Position::set(Entity entity, ecs::component::Position position) {
        auto& actualPosition = Registry::getComponent<component::Position>(entity);
        actualPosition.x = position.x;
        actualPosition.y = position.y;
    }
    
    void Position::move(Entity entity, ecs::component::Position offset) {
        auto& actualPosition = Registry::getComponent<component::Position>(entity);
        actualPosition.x += offset.x;
        actualPosition.y += offset.y;
    }
}
