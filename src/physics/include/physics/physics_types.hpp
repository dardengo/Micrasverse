#ifndef MICRASVERSE_PHYSICS_PHYSICS_TYPES_HPP
#define MICRASVERSE_PHYSICS_PHYSICS_TYPES_HPP

namespace micrasverse::physics {

    struct Vec2 {
        float x, y;
    };
    
    struct Rotation {
        float cosAngle, sinAngle;
    };
    
    struct Transform {
        Vec2 position;
        Rotation rotation;
    };


} // namespace micrasverse::physics

#endif // MICRASVERSE_PHYSICS_PHYSICS_TYPES_HPP
