#ifndef PHYSICS_ENGINE_HPP
#define PHYSICS_ENGINE_HPP

#include "physics/world.hpp"
#include "physics/maze.hpp"
#include "physics/micrasbody.hpp"

#include <string>

namespace micrasverse::physics {

class PhysicsEngine {
public:
    World p_World;
    Maze p_Maze;
    MicrasBody p_Micras;

    PhysicsEngine(const std::string& mazePath);

    void update(float step = 1.0f/60.0f);

};

}
#endif // PHYSICS_ENGINE_HPP
