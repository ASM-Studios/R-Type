#pragma once

#include <chrono>

namespace ecs::component {
    struct LastShot {
            std::chrono::steady_clock::time_point lastShotTime = std::chrono::steady_clock::now();
    };
}
