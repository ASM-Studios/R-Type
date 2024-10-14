#pragma once

#include "Entity.hpp"
#include <algorithm>
#include <optional>
#include <set>

namespace ecs::component {

    enum class Tag {
        Bullet,
        Enemy,
        Ally,
        Explosion,
    };

    struct Tags {
            std::set<Tag> tags;

            operator std::array<std::optional<Tag>, 4>() const {
                std::array<std::optional<Tag>, 4> arr;
                std::fill(arr.begin(), arr.end(), std::nullopt);
                std::copy(tags.begin(), tags.end(), arr.begin());
                return arr;
            }

            void addTag(Tag tag) {
                tags.insert(tag);
            }
            void removeTag(Tag tag) {
                tags.erase(tag);
            }

            static bool hasTag(const std::array<std::optional<Tag>, 4>& tags, Tag tag) {
                return std::find(tags.begin(), tags.end(), tag) != tags.end();
            }

            bool hasTag(Tag tag) const {
                return tags.find(tag) != tags.end();
            }

            bool operator==(const Tags& other) const {
                return tags == other.tags;
            }
    };
}
