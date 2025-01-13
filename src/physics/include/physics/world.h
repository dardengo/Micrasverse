#ifndef WORLD_H
#define WORLD_H

#include "box2d/box2d.h"
#include "core/constants.h"

namespace micrasverse::physics {

class World {
private:
    b2WorldId worldId;

public:
    explicit World(const b2Vec2 gravity);

    ~World();

    const b2WorldId getWorldId();

    void runStep(const float timeStep, const int subStepCount);

};

} // namespace micrasverse::physics

#endif // WORLD_H
