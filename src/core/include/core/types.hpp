#ifndef MICRAS_CORE_TYPES_HPP
#define MICRAS_CORE_TYPES_HPP

#include <cstdint>
#include <array>

namespace micrasverse::core {
enum Observation : uint8_t {
    UNKNOWN = 0,
    FREE_SPACE = 1,
    WALL = 2
};

enum FollowWallType : uint8_t {
    NONE = 0,
    FRONT = 1,
    LEFT = 2,
    RIGHT = 3,
    PARALLEL = 4,
    BACK = 5,
};

enum Objective : uint8_t {
    EXPLORE = 0,
    RETURN = 1,
    SOLVE = 2
};

struct Color {
    float r; // 0-1
    float g; // 0-1
    float b; // 0-1

    Color(uint8_t r, uint8_t g, uint8_t b) : r(r / 255.0f), g(g / 255.0f), b(b / 255.0f) {}

    constexpr Color(int r, int g, int b) : r(r), g(g), b(b) {}

    Color(float r, float g, float b) : r(r / 255.0f), g(g / 255.0f), b(b / 255.0f) {}

    //default constructor
    Color() : r(0), g(0), b(0) {}

    Color operator*(float brightness) const {
        return {static_cast<uint8_t>(r * brightness), static_cast<uint8_t>(g * brightness), static_cast<uint8_t>(b * brightness)};
    }    

    std::array<float, 3> toArray() const {
        return {static_cast<float>(r), static_cast<float>(g), static_cast<float>(b)};
    }
};

}  // namespace micras::core

#endif  // MICRAS_CORE_TYPES_HPP
