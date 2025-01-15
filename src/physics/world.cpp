#include "physics/world.hpp"

namespace micrasverse::physics {

    World::World(const b2Vec2 gravity) {
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = gravity;
        this->worldId = b2CreateWorld(&worldDef);
    }

    World::~World() {
        b2DestroyWorld(worldId);
        this->worldId = b2_nullWorldId;
    }

    const b2WorldId World::getWorldId() {
        return this->worldId;
    }

    void World::runStep(const float timeStep, const int subStepCount) {
        b2World_Step(this->worldId, timeStep, subStepCount);
    }

} // namespace micrasverse::physics
