#include "EntitySchematic.hpp"
#include "Position.hpp"

/**
 * @brief Create a player entity (client side)
 *
 * @param startX The start x position
 * @param startY The start y position
 * @param screenWidth The screen width
 * @param screenHeight The screen height
 * @return The player entity
 */
ecs::Entity EntitySchematic::createPlayer(int16_t startX, int16_t startY, int16_t screenWidth, int16_t screenHeight) {
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto player = registry.createEntity<>(0);
    ecs::component::Position const position = {startX, startY, static_cast<size_t>(screenWidth), static_cast<size_t>(screenHeight)};

    registry.setComponent<ecs::component::Tags>(player, ecs::component::Tags({ecs::component::Tag::Ally, ecs::component::Tag::Player}));
    registry.setComponent<ecs::component::Position>(player, position);
    registry.setComponent<ecs::component::Input>(player, {});
    registry.setComponent<ecs::component::Sprite>(player, {22, 2});
    registry.setComponent<ecs::component::LastShot>(player, {});
    registry.setComponent<ecs::component::Behavior>(player, {&BehaviorFunc::handleInput});
    return player;
}

/**
 * @brief Create a player entity (server side)
 *
 * @param id The entity id
 * @param x The x position
 * @param y The y position
 * @param spriteID The sprite id
 * @return The player entity
 */
ecs::Entity EntitySchematic::createTeamPlayer(uint64_t id, int16_t x, int16_t y, int spriteID) {
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto player = registry.createEntity<>(id);
    std::size_t a = std::stoi(Config::getInstance("a").get("width").value_or("1920"));
    ecs::component::Position position = {x, y, static_cast<std::size_t>(std::stoi(Config::getInstance("config/client.json").get("width").value_or("1920"))),
                                         static_cast<std::size_t>(std::stoi(Config::getInstance("config/client.json").get("height").value_or("1080")))};

    registry.setComponent<ecs::component::Tags>(player, ecs::component::Tags({ecs::component::Tag::Ally, ecs::component::Tag::Player}));
    registry.setComponent<ecs::component::Position>(player, position);
    registry.setComponent<ecs::component::Input>(player, {});
    registry.setComponent<ecs::component::Sprite>(player, {spriteID, 2});
    registry.setComponent<ecs::component::LastShot>(player, {});
    registry.setComponent<ecs::component::Behavior>(player, {&BehaviorFunc::handleInput});
    return player;
}

/**
 * @brief Create a player entity (client side)
 *
 * @param id The entity id
 * @param x The x position
 * @param y The y position
 * @param spriteID The sprite id
 * @return The player entity
 */
ecs::Entity EntitySchematic::createTeamPlayerClient(uint64_t id, int16_t x, int16_t y, int spriteID) {
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto player = registry.createEntity<>(id);
    ecs::component::Position position = {x, y, static_cast<std::size_t>(std::stoi(Config::getInstance("config/client.json").get("width").value_or("1920"))),
                                         static_cast<std::size_t>(std::stoi(Config::getInstance("config/client.json").get("height").value_or("1080")))};

    registry.setComponent<ecs::component::Tags>(player, ecs::component::Tags({ecs::component::Tag::Ally, ecs::component::Tag::Player}));
    registry.setComponent<ecs::component::Position>(player, position);
    registry.setComponent<ecs::component::Input>(player, {});
    registry.setComponent<ecs::component::Sprite>(player, {spriteID, 2});
    registry.setComponent<ecs::component::LastShot>(player, {});
    registry.setComponent<ecs::component::Behavior>(player, {&BehaviorFunc::setSpriteSheetFromInput});
    return player;
}

/**
 * @brief Create a player entity (server side)
 *
 * @param id The entity id
 * @param x The x position
 * @param y The y position
 * @param spriteID The sprite id
 * @return The player entity
 */
ecs::Entity EntitySchematic::createBullet() {
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto bullet = registry.createEntity<>();

    registry.setComponent<ecs::component::Tags>(bullet, ecs::component::Tags({ecs::component::Tag::Bullet, ecs::component::Tag::Ally}));
    registry.addComponent<ecs::component::Position>(bullet);
    registry.setComponent<ecs::component::Sprite>(bullet, {13, 0});
    registry.setComponent<ecs::component::Animation>(bullet, {});
    registry.setComponent<ecs::component::Behavior>(bullet, {&BehaviorFunc::updateBullet});
    registry.setComponent<ecs::component::Collision>(bullet, {ecs::component::CollisionAlgorithm::AABB});
    return bullet;
}

/**
 * @brief Create a bullet entity (client side)
 *
 * @param shooter The entity that shot the bullet
 * @return The bullet entity
 */
ecs::Entity EntitySchematic::createBullet(const ecs::Entity& shooter) {
    auto bullet = createBullet();
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto& shooterPosition = registry.getComponent<ecs::component::Position>(shooter);

    registry.setComponent<ecs::component::Position>(bullet, {static_cast<int16_t>(shooterPosition.x + 50), shooterPosition.y, shooterPosition.screenWidth, shooterPosition.screenHeight});
    return bullet;
}

/**
 * @brief Create an explosion entity
 *
 * @param destroyed The entity that was destroyed
 * @return The explosion entity
 */
ecs::Entity EntitySchematic::createExplosion(const ecs::Entity& destroyed) {
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto& destroyedPosition = registry.getComponent<ecs::component::Position>(destroyed);
    auto explosion = registry.createEntity<>();

    registry.setComponent<ecs::component::Tags>(explosion, ecs::component::Tags({ecs::component::Tag::Explosion}));
    registry.setComponent<ecs::component::Position>(explosion, {static_cast<int16_t>(destroyedPosition.x), destroyedPosition.y, destroyedPosition.screenWidth, destroyedPosition.screenHeight});
    registry.setComponent<ecs::component::Sprite>(explosion, {EXPLOSION_SPRITE_ID, 0});
    registry.setComponent<ecs::component::Animation>(explosion, {});

    return explosion;
}

/**
 * @brief Create an enemy entity
 *
 * @param id The entity id
 * @param x The x position
 * @param y The y position
 * @param spriteID The sprite id
 * @param stateID The state id
 * @param model The behavior model
 * @param screenSize The screen size
 * @return The enemy entity
 */
ecs::Entity EntitySchematic::createEnemy(std::size_t id, int16_t x, int16_t y, int spriteID, int stateID, const std::string& model, const std::pair<std::size_t, std::size_t>& screenSize) {
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto enemy = registry.createEntity<>();

    registry.setComponent<ecs::component::Tags>(enemy, ecs::component::Tags({ecs::component::Tag::Enemy}));
    registry.setComponent<ecs::component::Position>(enemy, {x, y, static_cast<size_t>(static_cast<int16_t>(screenSize.first)), static_cast<size_t>(static_cast<int16_t>(screenSize.second))});
    registry.setComponent<ecs::component::Sprite>(enemy, {spriteID, stateID});
    registry.setComponent<ecs::component::Behavior>(enemy, ecs::component::Behavior::stringToBehaviorModel(model));
    registry.setComponent<ecs::component::Animation>(enemy, {});
    return enemy;
}
