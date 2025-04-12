#ifndef CORE_TYPES_HPP
#define CORE_TYPES_HPP

#include <array>
#include "core/types.hpp"  // Import Color from main core module

namespace micrasverse::core {

// 2D Vector type
struct Vec2 {
    float x;
    float y;
    
    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float x, float y) : x(x), y(y) {}
};

// Rotation type
struct Rotation {
    float c; // cos
    float s; // sin
    
    Rotation() : c(1.0f), s(0.0f) {}
    Rotation(float cosine, float sine) : c(cosine), s(sine) {}
};

// Color type is now imported from core/types.hpp

// Observation enum is now imported from core/types.hpp

} // namespace micrasverse::core

#endif // CORE_TYPES_HPP 
