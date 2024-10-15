#pragma once

namespace ecs::component {
    struct Animation {
            int currFrame = 0;
            float elapsedTime = 0.0F;
    };
}
