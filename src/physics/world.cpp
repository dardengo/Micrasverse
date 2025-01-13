#include "physics/world.h"

namespace micrasverse::physics {

    World::World(const b2Vec2 gravity) {
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = gravity;
        worldId = b2CreateWorld(&worldDef);
    }

    World::~World() {
        b2DestroyWorld(worldId);
        worldId = b2_nullWorldId;
    }

    const b2WorldId World::getWorldId() {
        return worldId;
    }

    void World::runStep(const float timeStep, const int subStepCount) {
        b2World_Step(worldId, timeStep, subStepCount);
    }

} // namespace micrasverse::physics
