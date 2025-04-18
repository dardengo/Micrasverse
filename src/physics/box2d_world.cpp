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
    std::cout << "========== WORLD CREATION START ==========" << std::endl;
    
    // Check if a world already exists
    if (worldExists.exchange(true)) {
        std::cerr << "ERROR: Attempting to create a second world!" << std::endl;
        throw std::runtime_error("A Box2D world already exists. Only one world should be created during simulation.");
    }
    
    // Create world with error handling
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = micrasverse::GRAVITY;
    this->gravity = worldDef.gravity;
    
    std::cout << "Creating Box2D world..." << std::endl;
    
    // Create the Box2D world
    this->worldId = b2CreateWorld(&worldDef);
    
    // Debug info - print raw pointer address to avoid accessing internal fields
    std::cout << "World created with ID address: " << &this->worldId << std::endl;
    std::cout << "World index in debug info: " << this->worldId.index1 << std::endl;
    
    // Check if world creation succeeded
    bool isValid = b2World_IsValid(this->worldId);
    std::cout << "World validity check: " << (isValid ? "VALID" : "INVALID") << std::endl;
    
    if (!isValid) {
        worldExists = false; // Reset the flag if creation failed
        std::cerr << "ERROR: Failed to create Box2D world" << std::endl;
        throw std::runtime_error("Box2D world creation failed");
    }
    
    std::cout << "Created single Box2D world for simulation" << std::endl;
    std::cout << "========== WORLD CREATION END ==========" << std::endl;
}

World::~World() {
    std::cout << "========== WORLD DESTRUCTION START ==========" << std::endl;
    std::cout << "Destroying Box2D world with index: " << this->worldId.index1 << std::endl;
              
    if (b2World_IsValid(this->worldId)) {
        std::cout << "World is valid, proceeding with destruction" << std::endl;
        b2DestroyWorld(this->worldId);
        worldExists = false; // Reset the flag so a new world can be created if needed
        std::cout << "Box2D world successfully destroyed" << std::endl;
    } else {
        std::cerr << "WARNING: Attempting to destroy an invalid Box2D world" << std::endl;
    }
    std::cout << "========== WORLD DESTRUCTION END ==========" << std::endl;
}

b2WorldId World::getWorldId() const {
    std::cout << "Getting world ID, index: " << this->worldId.index1 << ", validity: " 
              << (b2World_IsValid(this->worldId) ? "VALID" : "INVALID") << std::endl;
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
