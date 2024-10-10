#pragma once

namespace ecs::component {
    struct Animation {
            int currFrame = 0;
            float frameTime = 0.2F;
            float elapsedTime = 0.0F;
    };
}
