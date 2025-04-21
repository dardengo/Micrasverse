#ifndef MICRASVERSE_PHYSICS_I_PHYSICS_ENGINE_HPP
#define MICRASVERSE_PHYSICS_I_PHYSICS_ENGINE_HPP

#include "physics/box2d_maze.hpp"
#include "physics/box2d_micrasbody.hpp"
#include "constants.hpp"
#include <string_view>

namespace micrasverse::physics {

class IPhysicsEngine {
public:
    IPhysicsEngine() = default;
    virtual ~IPhysicsEngine() = default;
    virtual void update(float step = micrasverse::STEP) = 0;
    virtual void loadMaze(const std::string_view mazePath) = 0;
    virtual void resetMicrasPosition() = 0;
    virtual Maze& getMaze() = 0;
    virtual Box2DMicrasBody& getMicras() = 0;
};

}

#endif // MICRASVERSE_PHYSICS_I_PHYSICS_ENGINE_HPP
