#ifndef PHYSICS_ENGINE_CPP
#define PHYSICS_ENGINE_CPP

#include "physics/physics_engine.hpp"
#include "box2d/box2d.h"
#include <filesystem>

namespace micrasverse::physics {

    PhysicsEngine::PhysicsEngine(const std::string& mazePath) :
        p_World(),
        p_Maze(p_World.getWorldId(), mazePath),
        p_Micras(p_World.getWorldId()) {}

    void PhysicsEngine::update(float step) {
        this->p_World.runStep(step, 4);
        this->p_Micras.update(step);
    }

    void PhysicsEngine::loadMaze(const std::string& mazePath) {
        this->p_Maze.reloadFromFile(mazePath);
    }

    void PhysicsEngine::resetMicrasPosition() {
        b2Body_SetTransform(p_Micras.getBodyId(), (b2Vec2){(CELL_SIZE+WALL_THICKNESS)/2.0f, (CELL_SIZE+WALL_THICKNESS)/2.0f}, (b2Rot){1.0f, 0.0f});
        b2Body_SetLinearVelocity(p_Micras.getBodyId(), (b2Vec2){0.0f, 0.0f});
        b2Body_SetAngularVelocity(p_Micras.getBodyId(), 0.0f);

        for (auto& argb : p_Micras.argb.argbs) {
            argb.update(); // or pass the body ID to use GetWorldPoint
        }
    }

}
#endif // PHYSICS_ENGINE_CPP
