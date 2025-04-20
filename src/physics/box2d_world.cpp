#include "physics/box2d_world.hpp"
#include "constants.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <cstring> // For memcpy

namespace micrasverse::physics {

// No need for world counting as we should only have one world
std::atomic<bool> World::worldExists(false);

World::World() {
    // Check if a world already exists
    if (worldExists.exchange(true)) {
        std::cerr << "ERROR: Attempting to create a second world!" << std::endl;
        throw std::runtime_error("A Box2D world already exists. Only one world should be created during simulation.");
    }

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = micrasverse::GRAVITY;
    this->gravity = worldDef.gravity;

    this->worldId = b2CreateWorld(&worldDef);
}

World::~World() {
    b2DestroyWorld(this->worldId);
    worldExists = false; // Reset the flag so a new world can be created if needed
}

b2WorldId World::getWorldId() const {
    return worldId;
}

void World::runStep(const float timeStep, const int subStepCount) {
    if (b2World_IsValid(this->worldId)) {
        b2World_Step(this->worldId, timeStep, subStepCount);
    } else {
        std::cerr << "WARNING: Attempting to step an invalid Box2D world, index: " << this->worldId.index1 << std::endl;
    }
}

} // namespace micrasverse::physics
