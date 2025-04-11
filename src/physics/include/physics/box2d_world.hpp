#ifndef WORLD_HPP
#define WORLD_HPP

#include "box2d/box2d.h"

namespace micrasverse::physics {

class World {
private:
    b2WorldId worldId;
    b2Vec2 gravity;

public:
    explicit World();

    ~World();

    const b2WorldId getWorldId();

    void runStep(const float timeStep, const int subStepCount);

};

} // namespace micrasverse::physics

#endif // WORLD_HPP
