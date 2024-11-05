#pragma once
#include <memory>
#include <vector>
#include "Registry.hpp"
#include "EntitySchematic.hpp"

class ProceduralGenerator {
    public:
    ProceduralGenerator(const std::pair<std::size_t, std::size_t>& screenSize);
    void update(float timePerTick);

    private:
    void spawnPlatform();
    std::pair<std::size_t, std::size_t> _screenSize;
    float _spawnInterval;
    float _distanceSinceLastSpawn;
};

