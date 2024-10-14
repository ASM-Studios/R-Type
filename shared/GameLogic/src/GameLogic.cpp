#include "GameLogic.hpp"
#include "Collision.hpp"
#include "Input.hpp"
#include "Position.hpp"
#include "RegistryManager.hpp"
#include "query/Payloads.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"
#include "socket/ServerManager.hpp"
#include <algorithm>

GameLogic::GameLogic(GameLogicMode mode) :
    _mode(mode),
    _timePerTick(1.0F / 60.0f) // Default values,
{
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

    auto &factory = ecs::factory::LevelFactory::getInstance();
    int const width = std::stoi(config.get("width").value_or("1920"));
    int const height = std::stoi(config.get("height").value_or("1080"));
    factory.load({width, height}, ecs::factory::getScenarioPath(1));
}

void GameLogic::start() {
    this->_isRunning = true;
}

void GameLogic::stop() {
    this->_isRunning = false;
}

void GameLogic::updateTimed() {
    if (!this->_isRunning) {
        return;
    }
    using clock = std::chrono::steady_clock;
    static auto prevTime = clock::now();
    static float tmp = 0.0F;

    auto cur = clock::now();
    float const deltaTime = std::chrono::duration<float>(cur - prevTime).count();
    prevTime = cur;
    tmp += deltaTime;
    _totalTime += deltaTime;
    if (tmp >= _timePerTick) {
        this->update();
        tmp -= _timePerTick;
    } else {
        float const sleepTime = _timePerTick - tmp;
        if (sleepTime > 0) {
            std::this_thread::sleep_for(std::chrono::duration<float>(sleepTime));
            tmp += sleepTime;
        }
    }
}

void GameLogic::update() {
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    for (const auto& entity: registry.getEntities()) {
        if (registry.contains<ecs::component::Behavior>(entity)) {
            registry.getComponent<ecs::component::Behavior>(entity).func(entity, _timePerTick);
        }
        if (this->_mode == CLIENT && registry.contains<ecs::component::Collision>(entity)) {
            registry.getComponent<ecs::component::Collision>(entity).checkCollision(entity);
        }
        this->server(entity);
        this->client(entity);
    }
    auto & factory = ecs::factory::LevelFactory::getInstance();
    factory.updateEntities(_totalTime);
}

void GameLogic::client(ecs::Entity entity) {
    if (this->_mode != CLIENT) {
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

void GameLogic::sendInput(ecs::Entity entity) {
    const Config& config = Config::getInstance("server/config.json");
    std::string hostname = config.get("hostname").value_or("127.0.0.1");
    int port = std::stoi(config.get("port").value_or("8080"));
    auto input = ecs::RegistryManager::getInstance().getRegistry().getComponent<ecs::component::Input>(entity);
    if (input.inputFlags == input.oldInputFlags) {
        return;
    }
    TypedQuery<ecs::component::Input> typedQuery(RequestType::INPUT, input);
    network::socket::udp::ServerManager::getInstance().getServer().send(hostname, port, RawRequest(typedQuery));
}

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

void GameLogic::server(ecs::Entity entity) {
    if (this->_mode != SERVER) {
        return;
    }
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    if (registry.contains<network::Client>(entity)) {
        this->sendPlayerPosition(entity);
        this->sendTeamPosition(entity);
    }
}

void GameLogic::sendPlayerPosition(ecs::Entity entity) {
    auto position = ecs::RegistryManager::getInstance().getRegistry().getComponent<ecs::component::Position>(entity);
    UpdatePlayer payload{position};

    RawRequest request(TypedQuery(RequestType::UPDATE_PLAYER, payload));
    auto client = ecs::RegistryManager::getInstance().getRegistry().getComponent<network::Client>(entity);
    network::socket::udp::ServerManager::getInstance().getServer().send(client.getIP().to_string(), client.getPort(), request);
}

void GameLogic::sendTeamPosition(ecs::Entity entity) {
    for (const auto& [destEntity, client]: ecs::RegistryManager::getInstance().getRegistry().getEntities<network::Client>()) {
        if (entity == destEntity) {
            continue;
        }
        UpdateTeamPlayer payload{entity.getID(), ecs::RegistryManager::getInstance().getRegistry().getComponent<ecs::component::Input>(entity), ecs::RegistryManager::getInstance().getRegistry().getComponent<ecs::component::Position>(entity)};
        TypedQuery<UpdateTeamPlayer> typedQuery(RequestType::UPDATE_TEAM_PLAYER, payload);
        network::socket::udp::ServerManager::getInstance().getServer().send(client.getIP().to_string(), client.getPort(), RawRequest(typedQuery));
    }
}
