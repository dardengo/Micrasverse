#ifndef BOX2D_PHYSICS_ENGINE_HPP
#define BOX2D_PHYSICS_ENGINE_HPP

#include "physics/i_physics_engine.hpp"
#include "config/constants.hpp"
#include "physics/box2d_world.hpp"
#include "physics/box2d_maze.hpp"
#include "physics/box2d_micrasbody.hpp"

#include <string>

namespace micrasverse::physics {

class Box2DPhysicsEngine : public IPhysicsEngine {
public:
    Box2DPhysicsEngine(const std::string& mazePath);

    void update(float step = micrasverse::STEP) override;

    void loadMaze(const std::string& mazePath) override;

    void resetMicrasPosition() override;

    Maze& getMaze() override;

    MicrasBody& getMicras() override;

private:
    World p_World;
    Maze p_Maze;
    MicrasBody p_Micras;
};

}
#endif // BOX2D_PHYSICS_ENGINE_HPP
