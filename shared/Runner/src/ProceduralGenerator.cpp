#include "ProceduralGenerator.hpp"
#include <random>

ProceduralGenerator::ProceduralGenerator(const std::pair<std::size_t, std::size_t>& screenSize)
    : _screenSize(screenSize), _spawnInterval(2), _distanceSinceLastSpawn(0)
{
    const int spriteID = 66;
    const int stateID = 0;
    std::pair<int, int> platformSize = TextureLoader::getInstance().getTexture(spriteID).getSize();

    int numPlatforms = _screenSize.first / platformSize.first;
    int16_t yPosition = _screenSize.second;

    std::shared_ptr<ecs::Registry> registry = ecs::RegistryManager::getInstance().getRegistry(0);
    if (!registry) {
        Logger::log(LogLevel::ERR, "Registry not initialized in ProceduralGenerator!");
        return;
    }
    for (int i = 0; i < numPlatforms * 2; ++i) {
        int16_t xPosition = i * platformSize.first;
        EntitySchematic::createPlatform(registry, registry->_generateID(), xPosition, yPosition, spriteID, stateID, _screenSize);
    }
}

void ProceduralGenerator::update(float timePerTick)
{
    _distanceSinceLastSpawn += timePerTick;
    if (_distanceSinceLastSpawn >= _spawnInterval) {
        _distanceSinceLastSpawn = 0;
        Logger::log(LogLevel::INFO, "Spawning platform");
        spawnPlatform();
    }
}

void ProceduralGenerator::spawnPlatform()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<> platformTypeDist(0, 1);
    bool const isLargePlatform = platformTypeDist(gen) == 1;

    int16_t const platformWidth = isLargePlatform ? 3 : 1;
    int const spriteID = isLargePlatform ? 66 : 65;
    int const stateID = 0;
    std::uniform_int_distribution<> yPosDist(_screenSize.second / 2, _screenSize.second - 50);
    int16_t const yPosition = yPosDist(gen);
    int16_t const xPosition = _screenSize.first + (TextureLoader::getInstance().getTexture(spriteID).getSize().first);
    std::shared_ptr<ecs::Registry> const registry = ecs::RegistryManager::getInstance().getRegistry(0);
    EntitySchematic::createPlatform(registry, registry->_generateID(), xPosition, yPosition, spriteID, stateID, _screenSize);
}

