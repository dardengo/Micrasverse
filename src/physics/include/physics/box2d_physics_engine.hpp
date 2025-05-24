#ifndef MICRASVERSE_PHYSICS_BOX2D_PHYSICS_ENGINE_HPP
#define MICRASVERSE_PHYSICS_BOX2D_PHYSICS_ENGINE_HPP

#include "physics/box2d_world.hpp"
#include "physics/box2d_maze.hpp"
#include "physics/box2d_micrasbody.hpp"
#include <memory>
#include <string>
#include <string_view>
#include "constants.hpp"

namespace micrasverse::physics {

class Box2DPhysicsEngine {
public:
    Box2DPhysicsEngine(const std::string_view mazePath = DEFAULT_MAZE_PATH);
    ~Box2DPhysicsEngine();

    void update(float step = STEP);
    void loadMaze(const std::string_view mazePath);
    void resetMicrasPosition();

    World& getWorld() { return *p_World; }

    Maze& getMaze() { return *p_Maze; }

    Box2DMicrasBody& getMicras() { return *p_Micras; }

private:
    std::unique_ptr<World>           p_World;
    std::unique_ptr<Maze>            p_Maze;
    std::unique_ptr<Box2DMicrasBody> p_Micras;
};

}  // namespace micrasverse::physics

#endif  // MICRASVERSE_PHYSICS_BOX2D_PHYSICS_ENGINE_HPP
