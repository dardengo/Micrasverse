#ifndef WORLD_HPP
#define WORLD_HPP

#include "config/constants.hpp"

#include "box2d/box2d.h"

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

#endif // WORLD_HPP
