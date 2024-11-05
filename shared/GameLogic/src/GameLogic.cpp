#include "GameLogic.hpp"
#include "Clock.hpp"
#include "Collision.hpp"
#include "Factories/LevelFactory.hpp"
#include "GameLogicMode.hpp"
#include "Input.hpp"
#include "Position.hpp"
#include "RegistryManager.hpp"
#include "Singleton.hpp"
#include "query/Payloads.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"
#include "socket/Client.hpp"
#include "socket/NetworkManager.hpp"
#include <algorithm>
#include <boost/stacktrace/stacktrace.hpp>

/**
 * @brief Construct a new GameLogic object
 *
 * @param mode The game logic mode
 */
GameLogic::GameLogic(const GameLogicMode mode) :
    _mode(mode),
    _timePerTick(1.0F / 60.0f),
    _totalTime(0) {
    const Config& config = Config::getInstance("server/config.json");
    int _tps = std::stoi(config.get("tps").value_or("60"));
    _timePerTick = 1.0F / static_cast<float>(_tps);
    TextureLoader::getInstance().loadFile("assets/textures_config.cfg");
    TextureLoader::getInstance().loadTextures("backgrounds", TextureLoader::Type::BACKGROUND);
    TextureLoader::getInstance().loadTextures("bullets", TextureLoader::Type::BULLET);
    TextureLoader::getInstance().loadTextures("enemies", TextureLoader::Type::ENEMY);
    TextureLoader::getInstance().loadTextures("ships", TextureLoader::Type::SHIP);
    TextureLoader::getInstance().loadTextures("explosions", TextureLoader::Type::EXPLOSION);
    TextureLoader::getInstance().loadTextures("player", TextureLoader::Type::PLAYER);
    TextureLoader::getInstance().loadTextures("platforms", TextureLoader::Type::PLATFORM);
    TextureLoader::getInstance().loadTextures("traps", TextureLoader::Type::TRAP);
    Logger::log(LogLevel::INFO, std::format("{0} textures have been loaded", TextureLoader::getInstance().getNoTexture()));

    int const width = std::stoi(config.get("width").value_or("1920"));
    int const height = std::stoi(config.get("height").value_or("1080"));
    if (isPureServer(this->_mode)) {
        for (int i = 0; i < 5; i++) {
            ecs::factory::LevelFactory factory({width, height}, ecs::factory::getScenarioPath(i));
            this->_levels.push_back(factory);
        }
    }
    if (isPureRunner(this->_mode)) {
        Logger::log(LogLevel::INFO, "Creating procedural generator");
        _proceduralGenerator = std::make_unique<ProceduralGenerator>((std::make_pair(width, height)));
    }
}

/**
 * @brief Update the game logic
 */
void GameLogic::updateTimed() {
    if (this->_clock.get() < this->_timePerTick) {
        return;
    }
    _totalTime += this->_clock.get() / 1000;
    this->_clock.reset();
    this->update();
}

/**
 * @brief Update the game logic
 */
void GameLogic::update() {
    for (auto registry: ecs::RegistryManager::getInstance().getRegistries()) {
        for (const auto& entity: registry->getEntities()) {
            if (registry->contains<ecs::component::Behavior>(entity)) {
                registry->getComponent<ecs::component::Behavior>(entity).func(this->_mode, entity, _timePerTick);
            }
            if(!isPureRunner(this->_mode)) {
                if (isPureClient(this->_mode) && registry->contains<ecs::component::Collision>(entity)) {
                    registry->getComponent<ecs::component::Collision>(entity).checkCollision(entity);
                }
                this->server(entity);
                this->client(entity);
            }
            else {
                if (registry->contains<ecs::component::Collision>(entity)) {
                    registry->getComponent<ecs::component::Collision>(entity).checkCollision(entity);
                }
            }
        }
    }
    if(!isPureRunner(this->_mode)) {
        for (auto& levelFactory: this->_levels) {
            levelFactory.updateEntities(this->_totalTime);
        }
    } else {
        _proceduralGenerator->update(_timePerTick);
    }
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
    auto registry = entity.getRegistry();
    if (registry->contains<ecs::component::Input>(entity)) {
        this->sendInput(entity);
    }
    if (registry->contains<ecs::component::Animation>(entity) && registry->contains<ecs::component::Sprite>(entity)) {
        this->updateAnimation(entity);
    }
}

/**
 * @brief Send the input to the server
 *
 * @param entity The entity to send the input
 */
void GameLogic::sendInput(const ecs::Entity& entity) {
    auto input = entity.getRegistry()->getComponent<ecs::component::Input>(entity);
    if (input.inputFlags == input.oldInputFlags) {
        return;
    }
    TypedQuery<ecs::component::Input> tq(RequestType::INPUT, input);
    {
        getServer().lock();
        network::socket::NetworkManager::getInstance().send(getServer().get(), RawRequest(tq), network::socket::Mode::UDP);
        getServer().unlock();
    }
}

/**
 * @brief Update the animation of an entity
 *
 * @param entity The entity to update
 */
void GameLogic::updateAnimation(const ecs::Entity& entity) {
    auto registry = entity.getRegistry();
    auto& animation = registry->getComponent<ecs::component::Animation>(entity);
    auto& sprite = registry->getComponent<ecs::component::Sprite>(entity);
    auto& texture = TextureLoader::getInstance();
    float const animation_speed = texture.getTexture(sprite.getSpriteID()).getAnimSpeed();
    auto& tags = registry->getComponent<ecs::component::Tags>(entity);
    animation.elapsedTime += _timePerTick;
    if (animation.elapsedTime >= animation_speed) {
        animation.elapsedTime -= animation_speed;
        animation.currFrame++;
        int const nbFrame = texture.getTexture(sprite.getSpriteID()).getFrameCount();
        if (animation.currFrame >= nbFrame) {
            if (tags.hasTag(ecs::component::Tag::Explosion)) {
                registry->removeEntity(entity);
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
    auto registry = entity.getRegistry();
    if (registry->contains<std::shared_ptr<network::Client>>(entity)) {
        this->sendPlayerPosition(entity);
        this->sendTeamPosition(entity);
    }
}

/**
 * @brief Send the player position to all clients
 *
 * @param entity The entity to send the position
 */
void GameLogic::sendPlayerPosition(const ecs::Entity& entity) {
    auto position = entity.getRegistry()->getComponent<ecs::component::Position>(entity);
    UpdatePlayer payload{position};

    RawRequest request(TypedQuery(RequestType::UPDATE_PLAYER, payload));
    auto client = entity.getRegistry()->getComponent<std::shared_ptr<network::Client>>(entity);
    network::socket::NetworkManager::getInstance().send(client, request, network::socket::Mode::UDP);
}

/**
 * @brief Send the team position to all clients
 *
 * @param entity The entity to send the position
 */
void GameLogic::sendTeamPosition(const ecs::Entity& entity) {
    for (const auto& [destEntity, destClient]: entity.getRegistry()->getEntities<std::shared_ptr<network::Client>>()) {
        if (entity == destEntity) {
            continue;
        }
        UpdateTeamPlayer payload{entity.getID(), entity.getRegistry()->getComponent<ecs::component::Input>(entity), entity.getRegistry()->getComponent<ecs::component::Position>(entity)};
        TypedQuery<UpdateTeamPlayer> typedQuery(RequestType::UPDATE_TEAM_PLAYER, payload);
        network::socket::NetworkManager::getInstance().send(destClient, RawRequest(typedQuery), network::socket::Mode::UDP);
    }
}
