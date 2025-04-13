#ifndef MICRASVERSE_PHYSICS_BOX2D_PHYSICS_ENGINE_HPP
#define MICRASVERSE_PHYSICS_BOX2D_PHYSICS_ENGINE_HPP

#include "physics/i_physics_engine.hpp"
#include "physics/box2d_world.hpp"
#include "physics/box2d_maze.hpp"
#include "physics/box2d_micrasbody.hpp"
#include <memory>
#include <string>

namespace micrasverse::physics {

class Box2DPhysicsEngine : public IPhysicsEngine {
public:
    Box2DPhysicsEngine(const std::string& mazePath);
    ~Box2DPhysicsEngine() override;

    void update(float step = micrasverse::STEP) override;
    void loadMaze(const std::string& mazePath) override;
    void resetMicrasPosition() override;
    World& getWorld() { return *p_World; }
    Maze& getMaze() override { return *p_Maze; }
    Box2DMicrasBody& getMicras() override { return *p_Micras; }

private:
    void initializeComponents();
    std::unique_ptr<World> p_World;
    std::unique_ptr<Maze> p_Maze;
    std::unique_ptr<Box2DMicrasBody> p_Micras;
};

} // namespace micrasverse::physics

#endif // MICRASVERSE_PHYSICS_BOX2D_PHYSICS_ENGINE_HPP
