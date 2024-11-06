#include "ProceduralGenerator.hpp"
#include "Logger.hpp"
#include "Texture.hpp"
#include "TextureLoader.hpp"
#include <format>
#include <random>
#include <vector>
#include <memory>

ProceduralGenerator::ProceduralGenerator(const std::pair<std::size_t, std::size_t>& screenSize)
    : _screenSize(screenSize), _lastRoomEndX(screenSize.first)
{
    _generateInitialPlatforms();
}

void ProceduralGenerator::_generateInitialPlatforms()
{
    for (int i = 0; i < 5; ++i) {
        _generatePlatform();
    }
}

void ProceduralGenerator::_generatePlatform()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> platformTypeDist(1, 2);
    int platformHeightMin = _screenSize.second / 2;
    int platformHeightMax = _screenSize.second - TextureLoader::getInstance().getSizeFromId(65).second;
    static std::uniform_int_distribution<> heightDist(platformHeightMin / TextureLoader::getInstance().getSizeFromId(65).second,
                                                  platformHeightMax / TextureLoader::getInstance().getSizeFromId(65).second);
    static std::uniform_int_distribution<> gapDist( TextureLoader::getInstance().getSizeFromId(65).first, 300);

    std::shared_ptr<ecs::Registry> registry = ecs::RegistryManager::getInstance().getRegistry(0);
    if (!registry) {
        Logger::log(LogLevel::ERR, "Registry not initialized in ProceduralGenerator!");
        return;
    }

    int platformType = platformTypeDist(gen);
    int platformHeight = heightDist(gen) * TextureLoader::getInstance().getSizeFromId(65).second;
    int gap = gapDist(gen);
    int16_t xPosition = _lastRoomEndX + gap;
    _lastRoomEndX = xPosition + (platformType == 1 ? TextureLoader::getInstance().getSizeFromId(65).first
                                                   : TextureLoader::getInstance().getSizeFromId(66).first);

    if (platformType == 1) {
        EntitySchematic::createPlatform(registry, registry->_generateID(), xPosition, platformHeight, 65, 0, _screenSize);
    } else {
        EntitySchematic::createPlatform(registry, registry->_generateID(), xPosition, platformHeight, 66, 0, _screenSize);
    }
}

void ProceduralGenerator::update(float timePerTick)
{
    _lastRoomEndX += -400 / 2 * timePerTick;
    if (_lastRoomEndX <= _screenSize.first + 100) {
        _generatePlatform();
    }
}
