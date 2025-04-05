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

}
#endif // PHYSICS_ENGINE_CPP
