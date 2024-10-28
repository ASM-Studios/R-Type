#include "GameLogic.hpp"
#include "Client.hpp"
#include "Clock.hpp"
#include "Collision.hpp"
#include "Factories/LevelFactory.hpp"
#include "GameLogicMode.hpp"
#include "Input.hpp"
#include "Position.hpp"
#include "RegistryManager.hpp"
#include "query/Payloads.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"
#include "socket/ServerManager.hpp"
#include <algorithm>

/**
 * @brief Construct a new GameLogic object
 *
 * @param mode The game logic mode
 */
GameLogic::GameLogic(const GameLogicMode mode) :
    _mode(mode), _isRunning(false),
    _timePerTick(1.0F / 60.0f) {
    const Config& config = Config::getInstance("server/config.json");
    int _tps = std::stoi(config.get("tps").value_or("60"));
    _timePerTick = 1.0F / static_cast<float>(_tps);
    TextureLoader::getInstance().loadFile("assets/textures_config.cfg");
    TextureLoader::getInstance().loadTextures("backgrounds", TextureLoader::Type::BACKGROUND);
    TextureLoader::getInstance().loadTextures("bullets", TextureLoader::Type::BULLET);
    TextureLoader::getInstance().loadTextures("enemies", TextureLoader::Type::ENEMY);
    TextureLoader::getInstance().loadTextures("ships", TextureLoader::Type::SHIP);
    TextureLoader::getInstance().loadTextures("explosions", TextureLoader::Type::EXPLOSION);
    Logger::log(LogLevel::INFO, std::format("{0} textures have been loaded", TextureLoader::getInstance().getNoTexture()));

    auto& factory = ecs::factory::LevelFactory::getInstance();
    int const width = std::stoi(config.get("width").value_or("1920"));
    int const height = std::stoi(config.get("height").value_or("1080"));
    factory.load({width, height}, ecs::factory::getScenarioPath(1));
}

/**
 * @brief Start the game logic
 */
void GameLogic::start() {
    this->_total.reset();
    this->_isRunning = true;
}

/**
 * @brief Stop the game logic
 */
void GameLogic::stop() {
    this->_isRunning = false;
}

/**
 * @brief Update the game logic
 */
void GameLogic::updateTimed() {
    if (!this->_isRunning) {
        return;
    }

    if (this->_clock.get() < this->_timePerTick) {
        return;
    }
    this->_clock.reset();
    this->update();
}

/**
 * @brief Update the game logic
 */
void GameLogic::update() {
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    for (const auto& entity: registry.getEntities()) {
        if (registry.contains<ecs::component::Behavior>(entity)) {
            registry.getComponent<ecs::component::Behavior>(entity).func(this->_mode, entity, _timePerTick);
        }
        if (isPureClient(this->_mode) && registry.contains<ecs::component::Collision>(entity)) {
            registry.getComponent<ecs::component::Collision>(entity).checkCollision(entity);
        }
        this->server(entity);
        this->client(entity);
    }
    auto& factory = ecs::factory::LevelFactory::getInstance();
    factory.updateEntities(static_cast<float>(this->_total.get()) / 1000);
}

/**
 * @brief Update the game logic for a client
 *
 * @param entity The entity to update
 */
void GameLogic::client(const ecs::Entity& entity) {
    if (!isPureClient(this->_mode)) {
        return;
    }
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    if (registry.contains<ecs::component::Input>(entity)) {
        this->sendInput(entity);
    }
    if (registry.contains<ecs::component::Animation>(entity) && registry.contains<ecs::component::Sprite>(entity)) {
        this->updateAnimation(entity);
    }
}

/**
 * @brief Send the input to the server
 *
 * @param entity The entity to send the input
 */
void GameLogic::sendInput(const ecs::Entity& entity) {
    auto input = ecs::RegistryManager::getInstance().getRegistry().getComponent<ecs::component::Input>(entity);
    if (input.inputFlags == input.oldInputFlags) {
        return;
    }
    TypedQuery<ecs::component::Input> typedQuery(RequestType::INPUT, input);

}

/**
 * @brief Update the animation of an entity
 *
 * @param entity The entity to update
 */
void GameLogic::updateAnimation(const ecs::Entity& entity) {
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto& animation = registry.getComponent<ecs::component::Animation>(entity);
    auto& sprite = registry.getComponent<ecs::component::Sprite>(entity);
    auto& texture = TextureLoader::getInstance();
    float const animation_speed = texture.getTexture(sprite.getSpriteID()).getAnimSpeed();
    auto& tags = registry.getComponent<ecs::component::Tags>(entity);
    animation.elapsedTime += _timePerTick;
    if (animation.elapsedTime >= animation_speed) {
        animation.elapsedTime -= animation_speed;
        animation.currFrame++;
        int const nbFrame = texture.getTexture(sprite.getSpriteID()).getFrameCount();
        if (animation.currFrame >= nbFrame) {
            if (tags.hasTag(ecs::component::Tag::Explosion)) {
                registry.removeEntity(entity);
                return;
            }
            animation.currFrame = 0;
        }
        sprite.setStateID(animation.currFrame);
    }
}

/**
 * @brief Update the game logic for a server
 *
 * @param entity The entity to update
 */
void GameLogic::server(const ecs::Entity& entity) {
    if (!isPureServer(this->_mode)) {
        return;
    }
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    if (registry.contains<network::Client>(entity)) {
        this->sendPlayerPosition(entity);
        this->sendTeamPosition(entity);
        this->sendEntityPosition(entity);
    }
}

/**
 * @brief Send the player position to all clients
 *
 * @param entity The entity to send the position
 */
void GameLogic::sendPlayerPosition(const ecs::Entity& entity) {
    auto position = ecs::RegistryManager::getInstance().getRegistry().getComponent<ecs::component::Position>(entity);
    UpdatePlayer payload{position};

    RawRequest request(TypedQuery(RequestType::UPDATE_PLAYER, payload));
    auto client = ecs::RegistryManager::getInstance().getRegistry().getComponent<network::Client>(entity);
    network::socket::ServerManager::getInstance().getServer().send(client.getIP().to_string(), client.getPort(), request);
}

/**
 * @brief Send the team position to all clients
 *
 * @param entity The entity to send the position
 */
void GameLogic::sendTeamPosition(const ecs::Entity& entity) {
    for (const auto& [destEntity, destClient]: ecs::RegistryManager::getInstance().getRegistry().getEntities<network::Client>()) {
        if (entity == destEntity) {
            continue;
        }
        UpdateTeamPlayer payload{entity.getID(), ecs::RegistryManager::getInstance().getRegistry().getComponent<ecs::component::Input>(entity), ecs::RegistryManager::getInstance().getRegistry().getComponent<ecs::component::Position>(entity)};
        TypedQuery<UpdateTeamPlayer> typedQuery(RequestType::UPDATE_TEAM_PLAYER, payload);
        network::socket::ServerManager::getInstance().getServer().send(destClient.getIP().to_string(), destClient.getPort(), RawRequest(typedQuery));
    }
}

void GameLogic::sendEntityPosition(const ecs::Entity& entity) {
    for (const auto& NPEntity: ecs::RegistryManager::getInstance().getRegistry().getEntities()) {
        TypedQuery<UpdateEntity> tq(RequestType::UPDATE_ENTITY, {NPEntity.getID(), ecs::RegistryManager::getInstance().getRegistry().getComponent<ecs::component::Position>(NPEntity)});
        network::socket::ServerManager::getInstance().getServer().sendAll(ecs::RegistryManager::getInstance().getRegistry().getComponents<network::Client>(), RawRequest(tq));
    }
}
