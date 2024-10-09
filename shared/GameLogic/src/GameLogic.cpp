#include "GameLogic.hpp"

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
        if (entity.getType() == ecs::EntityType::Player
            && registry.contains<ecs::component::Input>(entity)
            && registry.contains<ecs::component::Position>(entity) ) {
            handleInput(entity);
        }
    }
    updateBullets();
}


void GameLogic::handleInput(const ecs::Entity& entity)
{
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    auto& input = registry.getComponent<ecs::component::Input>(entity);
    auto& position = registry.getComponent<ecs::component::Position>(entity);
    auto& lastShot = registry.getComponent<ecs::component::LastShot>(entity);

    ecs::component::Position offset(0, 0);
    if (input.isFlagSet(ecs::component::Input::MoveLeft)) {
        offset.x -= SPEED;
    }
    if (input.isFlagSet(ecs::component::Input::MoveRight)) {
        offset.x += SPEED;
    }
    if (input.isFlagSet(ecs::component::Input::MoveUp)) {
        offset.y -= SPEED;
    }
    if (input.isFlagSet(ecs::component::Input::MoveDown)) {
        offset.y += SPEED;
    }
    position.move(offset);

    auto currentTime = std::chrono::steady_clock::now();
    float const deltaTimeShot = std::chrono::duration<float>(currentTime - lastShot.lastShotTime).count();

    if (input.isFlagSet(ecs::component::Input::PressedShoot) && deltaTimeShot >= SHOOT_COOLDOWN
        && registry.contains<ecs::component::LastShot>(entity) ) {
        EntitySchematic::createBullet(entity);
        lastShot.lastShotTime = currentTime;
        input.clearFlag(ecs::component::Input::PressedShoot);
    }
}



void GameLogic::updateBullets()
{
    Collision::updateCollisions();
    ecs::Registry& registry = ecs::RegistryManager::getInstance().getRegistry();
    for (const auto& bullet : registry.getEntities()) {
        if (bullet.getType() == ecs::EntityType::Bullet
            && registry.contains<ecs::component::Position>(bullet)
            && registry.contains<ecs::component::Sprite>(bullet)) {
            auto& position = registry.getComponent<ecs::component::Position>(bullet);
            ecs::component::Position const offset(SPEED * 2, 0);

            position.move(offset);
            if (position.x >= position.screenWidth - 1) {
                registry.removeEntity(bullet);
            }
        }
    }
}



