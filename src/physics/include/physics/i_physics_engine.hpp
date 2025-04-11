#ifndef I_PHYSICS_ENGINE_HPP
#define I_PHYSICS_ENGINE_HPP

#include "physics/box2d_maze.hpp"
#include "physics/box2d_micrasbody.hpp"
#include "config/constants.hpp"
#include <string>

namespace micrasverse::physics {

class IPhysicsEngine {
public:
    IPhysicsEngine() = default;
    virtual ~IPhysicsEngine() = default;
    virtual void update(float step = micrasverse::STEP) = 0;
    virtual void loadMaze(const std::string& mazePath) = 0;
    virtual void resetMicrasPosition() = 0;
    virtual Maze& getMaze() = 0;
    virtual MicrasBody& getMicras() = 0;
};

}

#endif // I_PHYSICS_ENGINE_HPP
