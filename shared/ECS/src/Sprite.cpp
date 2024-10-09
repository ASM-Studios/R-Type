#include "Sprite.hpp"

namespace ecs::component {
    void Sprite::setSpriteID(int spriteID) {
        this->_spriteID = spriteID;
    }

    void Sprite::setStateID(int stateID) {
        this->_stateID = stateID;
    }

    int Sprite::getSpriteID() const {
        return this->_spriteID;
    }

    int Sprite::getStateID() const {
        return this->_stateID;
    }

    std::pair<int, int> Sprite::getSpriteState() const {
        return std::make_pair(this->_spriteID, this->_stateID);
    }
}
