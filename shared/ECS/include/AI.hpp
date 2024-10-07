#pragma once

#include <string>

namespace ecs::component {
    enum AIModel { // Update stringToAIModel if you add more models
        NONE = 0,
    };

    struct AI {
        AIModel _model = AIModel::NONE;

        void setModel(const AIModel model);

        static AIModel stringToAIModel(const std::string& modelStr);
    };
}
