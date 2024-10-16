#pragma once
#include <cstdint>

namespace ecs::component {
    struct Input {
            uint32_t oldInputFlags = 0;
            uint32_t inputFlags = 0;
            static constexpr uint32_t MoveLeft = 0x01;
            static constexpr uint32_t MoveRight = 0x02;
            static constexpr uint32_t MoveUp = 0x04;
            static constexpr uint32_t MoveDown = 0x08;

            static constexpr uint32_t PressedShoot = 0x10;
            static constexpr uint32_t ReleaseShoot = 0x20;

            Input& operator=(const Input& other) {
                this->oldInputFlags = this->inputFlags;
                this->inputFlags = other.inputFlags;
                return *this;
            }

            void setFlag(uint32_t flag) {
                inputFlags |= flag;
            }

            void clearFlag(uint32_t flag) {
                inputFlags &= ~flag;
            }

            [[nodiscard]] bool isFlagSet(uint32_t flag) const {
                return (inputFlags & flag) != 0;
            }

            void reset() {
                inputFlags = 0;
            }
    };
}
