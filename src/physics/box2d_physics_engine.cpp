#ifndef BOX2D_PHYSICS_ENGINE_CPP
#define BOX2D_PHYSICS_ENGINE_CPP

#include "physics/box2d_physics_engine.hpp"
#include "box2d/box2d.h"
#include <filesystem>

namespace micrasverse::physics {

    Box2DPhysicsEngine::Box2DPhysicsEngine(const std::string& mazePath) :
        p_World(),
        p_Maze(p_World.getWorldId(), mazePath),
        p_Micras(p_World.getWorldId()) {}

    void Box2DPhysicsEngine::update(float step) {
        this->p_World.runStep(step, 4);
        this->p_Micras.update(step);
    }

    void Box2DPhysicsEngine::loadMaze(const std::string& mazePath) {
        this->p_Maze.reloadFromFile(mazePath);
    }

    void Box2DPhysicsEngine::resetMicrasPosition() {
        b2Body_SetTransform(p_Micras.getBodyId(), (b2Vec2){(CELL_SIZE+WALL_THICKNESS)/2.0f, (CELL_SIZE+WALL_THICKNESS)/2.0f}, (b2Rot){1.0f, 0.0f});
        b2Body_SetLinearVelocity(p_Micras.getBodyId(), (b2Vec2){0.0f, 0.0f});
        b2Body_SetAngularVelocity(p_Micras.getBodyId(), 0.0f);

        for (auto& argb : p_Micras.argb.argbs) {
            argb.update(); // or pass the body ID to use GetWorldPoint
        }
    }

    Maze& Box2DPhysicsEngine::getMaze() {
        return this->p_Maze;
    }

    MicrasBody& Box2DPhysicsEngine::getMicras() {
        return this->p_Micras;
    }

}
#endif // BOX2D_PHYSICS_ENGINE_CPP
