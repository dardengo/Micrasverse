#include "physics/box2d_world.hpp"
#include "config/constants.hpp"

namespace micrasverse::physics {

    World::World() {
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = micrasverse::GRAVITY;
        this->gravity = worldDef.gravity;
        this->worldId = b2CreateWorld(&worldDef);
    }

    World::~World() {
        b2DestroyWorld(worldId);
    }

    const b2WorldId World::getWorldId() {
        return this->worldId;
    }

    void World::runStep(const float timeStep, const int subStepCount) {
        b2World_Step(this->worldId, timeStep, subStepCount);
    }

} // namespace micrasverse::physics
