#pragma once
#include "EnemyBehavior.hpp"
#include "Entity.hpp"
#include "Logger.hpp"

namespace ecs::component {
    enum AIModel {
        NONE,
        CLASSIC,
        SINUSOIDAL,
        ZIG_ZAG
    };

    const std::map<std::string, AIModel> stringToAIModel = {
        {"none", AIModel::NONE},
        {"classic", AIModel::CLASSIC},
        {"sinusoidal", AIModel::SINUSOIDAL},
        {"zig-zag", AIModel::ZIG_ZAG}
    };

    const std::map<AIModel, std::string> AIModelToString = {
        {AIModel::NONE, "none"},
        {AIModel::CLASSIC, "classic"},
        {AIModel::SINUSOIDAL, "sinusoidal"},
        {AIModel::ZIG_ZAG, "zig-zag"}
    };

    struct Behavior {
            std::function<void(GameLogicMode mode, const ecs::Entity entity, float timePerTick)> func;
            static Behavior stringToBehaviorModel(const std::string& modelStr);
    };
}
