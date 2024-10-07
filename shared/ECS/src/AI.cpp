#include "AI.hpp"

#include <Logger.hpp>

namespace ecs::component {
    void AI::setModel(const AIModel model) {
        _model = model;
    }

    AIModel AI::stringToAIModel(const std::string& modelStr) {
        if (modelStr == "none") {
            return AIModel::NONE;
        }
        // TODO: Add more AI models here
        Logger::log(LogLevel::ERR, "Unknown AI model: " + modelStr);
        return AIModel::NONE;
    }
}
