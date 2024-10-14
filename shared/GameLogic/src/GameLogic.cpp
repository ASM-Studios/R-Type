#include "GameLogic.hpp"
#include "Collision.hpp"

GameLogic::GameLogic()
    : _timePerTick(1.0F / 60.0f) // Default values
{
    const Config& config = Config::getInstance("client/config.json");
    int _tps = std::stoi(config.get("tps").value_or("60"));
    _timePerTick = 1.0F / static_cast<float>(_tps);
    TextureLoader::getInstance().loadFile("assets/textures_config.cfg");
    TextureLoader::getInstance().loadTextures("backgrounds", TextureLoader::Type::BACKGROUND);
    TextureLoader::getInstance().loadTextures("bullets", TextureLoader::Type::BULLET);
    TextureLoader::getInstance().loadTextures("enemies", TextureLoader::Type::ENEMY);
    TextureLoader::getInstance().loadTextures("ships", TextureLoader::Type::SHIP);
    TextureLoader::getInstance().loadTextures("explosions", TextureLoader::Type::EXPLOSION);
    Logger::log(LogLevel::INFO, std::format("{0} textures have been loaded", TextureLoader::getInstance().getNoTexture()));
}

void GameLogic::updateTimed()
{
    using clock = std::chrono::steady_clock;
    static auto prevTime = clock::now();
    static float tmp = 0.0F;

    auto cur = clock::now();
    float const deltaTime = std::chrono::duration<float>(cur - prevTime).count();
    prevTime = cur;
    tmp += deltaTime;

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
    for (const auto& entity : registry.getEntities()) {
        if (registry.contains<ecs::component::Animation>(entity) &&
            registry.contains<ecs::component::Sprite>(entity)) {
            updateAnimation(entity);
        }
        if (registry.contains<ecs::component::Behavior>(entity)) {
            registry.getComponent<ecs::component::Behavior>(entity).func(entity, _timePerTick);
        }
        if (registry.contains<ecs::component::Collision>(entity)) {
            registry.getComponent<ecs::component::Collision>(entity).checkCollision(entity);
        }
    }
}

void GameLogic::updateAnimation (const ecs::Entity& entity)
{
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

