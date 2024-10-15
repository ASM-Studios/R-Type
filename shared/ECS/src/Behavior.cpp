#include "Behavior.hpp"

namespace ecs::component {
    Behavior Behavior::stringToBehaviorModel(const std::string& modelStr)
    {
        if (modelStr == "none") {
            return {&EnemyBehavior::classicEnemy};
        }
        if (modelStr == "classic") {
            return {&EnemyBehavior::classicEnemy};
        }
        if (modelStr == "sinusoidal") {
            return {&EnemyBehavior::EnemySinusoidal};
        }
        if (modelStr == "zig-zag") {
            return {&EnemyBehavior::EnemyZigZag};
        }
        // TODO: Add more AI models here
        Logger::log(LogLevel::ERR, "Unknown Behavior model: " + modelStr);
        return {&EnemyBehavior::classicEnemy};
    }
}
