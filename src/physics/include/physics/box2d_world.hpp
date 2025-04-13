#ifndef WORLD_HPP
#define WORLD_HPP

#include "box2d/box2d.h"
#include <stdexcept>
#include <atomic>
#include <string>

namespace micrasverse::physics {

class World {
private:
    b2WorldId worldId;
    b2Vec2 gravity;
    
    // Flag to ensure only one world exists at a time
    static std::atomic<bool> worldExists;

public:
    explicit World();
    ~World();

    b2WorldId getWorldId() const;
    void runStep(const float timeStep, const int subStepCount);
    
    // Check if a world already exists
    static bool hasExistingWorld() { return worldExists; }
};

} // namespace micrasverse::physics

#endif // WORLD_HPP
