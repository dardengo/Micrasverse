#ifndef MICRASVERSE_CORE_TYPES_HPP
#define MICRASVERSE_CORE_TYPES_HPP

#include <cstdint>
#include <array>
#include <cmath>

namespace micrasverse::types {

class Vec2 {
public:
    float x, y;

    // Constructors
    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float x, float y) : x(x), y(y) {}

    // Basic vector operations
    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }

    Vec2 operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    Vec2 operator/(float scalar) const {
        return Vec2(x / scalar, y / scalar);
    }

    // Dot product
    float dot(const Vec2& other) const {
        return x * other.x + y * other.y;
    }

    // Length operations
    float length() const {
        return std::sqrt(x * x + y * y);
    }

    float lengthSquared() const {
        return x * x + y * y;
    }

    // Normalization
    Vec2 normalize() const {
        float len = length();
        if (len > 0) {
            return *this / len;
        }
        return *this;
    }
};

struct Rotation {
    float c; // cos
    float s; // sin
    
    Rotation() : c(1.0f), s(0.0f) {}
    Rotation(float cosine, float sine) : c(cosine), s(sine) {}
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

/**
 * @brief Predefined colors.
 */
struct Colors {
    Colors() = delete;

    static constexpr Color red{255, 0, 0};
    static constexpr Color green{0, 255, 0};
    static constexpr Color blue{0, 0, 255};
    static constexpr Color yellow{255, 255, 0};
    static constexpr Color cyan{0, 255, 255};
    static constexpr Color magenta{255, 0, 255};
    static constexpr Color white{255, 255, 255};
};

}  // namespace micrasverse::types

#endif  // MICRASVERSE_CORE_TYPES_HPP
