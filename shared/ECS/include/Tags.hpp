#pragma once

#include "Entity.hpp"

namespace ecs::component {

    enum class Tag {
        Bullet,
        Enemy,
        Ally,
        Explosion,
    };

    struct Tags {
        std::set<Tag> tags;
        void addTag(Tag tag) {
            tags.insert(tag);
        }
        void removeTag(Tag tag) {
            tags.erase(tag);
        }
        bool hasTag(Tag tag) const {
            return tags.find(tag) != tags.end();
        }

        bool operator==(const Tags& other) const {
            return tags == other.tags;
        }
    };
}