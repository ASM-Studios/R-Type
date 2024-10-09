#pragma once

#include <utility>

namespace ecs::component {
    struct Sprite {
        int _spriteID;
        int _stateID;
        void setSpriteID(int spriteID);
        void setStateID(int stateID);
        int getSpriteID() const;
        int getStateID() const;
        std::pair<int, int> getSpriteState() const;
    };
}
