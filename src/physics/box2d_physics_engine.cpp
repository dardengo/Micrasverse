#ifndef BOX2D_PHYSICS_ENGINE_CPP
#define BOX2D_PHYSICS_ENGINE_CPP

#include "physics/box2d_physics_engine.hpp"
#include "box2d/box2d.h"
#include "micras/proxy/wall_sensors.hpp"
#include "micras/proxy/locomotion.hpp"
#include "micras/proxy/argb.hpp"
#include "micras/proxy/dip_switch.hpp"
#include "constants.hpp"
#include <filesystem>
#include <stdexcept>
#include <iostream>

namespace micrasverse::physics {

    Box2DPhysicsEngine::Box2DPhysicsEngine(const std::string& mazePath) {
        std::cout << "========== PHYSICS ENGINE INITIALIZATION START ==========" << std::endl;
        
        try {
            std::cout << "Creating Box2D world..." << std::endl;
            // Create a single world for this physics engine
            p_World = std::make_unique<World>();
            
            // Store a single copy of worldId instead of regenerating it
            b2WorldId worldId = p_World->getWorldId();
            std::cout << "Physics engine obtained world ID with index: " << worldId.index1 << std::endl;
            
            // Verify the world was created successfully
            bool isValidWorld = b2World_IsValid(worldId);
            std::cout << "World validity check: " << (isValidWorld ? "VALID" : "INVALID") << std::endl;
            
            if (!isValidWorld) {
                std::cerr << "ERROR: World creation failed or produced an invalid world" << std::endl;
                throw std::runtime_error("Invalid Box2D world after creation");
            }
            
            std::cout << "Box2D world created successfully" << std::endl;
            
            // Create Maze
            try {
                std::cout << "Creating Maze with world ID index: " << worldId.index1 << std::endl;
                bool isStillValid = b2World_IsValid(worldId);
                std::cout << "World validity before creating Maze: " << (isStillValid ? "VALID" : "INVALID") << std::endl;
                
                // Use the same worldId reference
                p_Maze = std::make_unique<Maze>(worldId, mazePath);
                std::cout << "Maze created successfully" << std::endl;
                
                // Check world ID validity after maze creation
                worldId = p_World->getWorldId(); // Get fresh copy
                isStillValid = b2World_IsValid(worldId);
                std::cout << "World validity after creating Maze: " << (isStillValid ? "VALID" : "INVALID") << std::endl;
                
            } catch (const std::exception& e) {
                std::cerr << "Failed to create maze: " << e.what() << std::endl;
                throw;
            }
            
            // Create Micras body
            try {
                std::cout << "Creating MicrasBody with world ID index: " << worldId.index1 << std::endl;
                bool isStillValid = b2World_IsValid(worldId);
                std::cout << "World validity before creating MicrasBody: " << (isStillValid ? "VALID" : "INVALID") << std::endl;
                
                // Use the same worldId reference
                p_Micras = std::make_unique<Box2DMicrasBody>(
                    worldId,
                    b2Vec2((CELL_SIZE+WALL_THICKNESS)/2.0f, (CELL_SIZE+WALL_THICKNESS)/2.0f),
                    b2Vec2(MICRAS_WIDTH, MICRAS_HEIGHT),
                    b2_dynamicBody,
                    MICRAS_MASS,
                    MICRAS_FRICTION,
                    MICRAS_RESTITUTION
                );
                std::cout << "MicrasBody created successfully" << std::endl;
                
                // Check world ID validity after micras creation
                worldId = p_World->getWorldId(); // Get fresh copy
                isStillValid = b2World_IsValid(worldId);
                std::cout << "World validity after creating MicrasBody: " << (isStillValid ? "VALID" : "INVALID") << std::endl;
                
            } catch (const std::exception& e) {
                std::cerr << "Failed to create Micras body: " << e.what() << std::endl;
                throw;
            }
            
            // Initialize the components
            std::cout << "Initializing Micras components..." << std::endl;
            initializeComponents();
            std::cout << "Components initialized successfully" << std::endl;
            
        } catch (const std::exception& e) {
            std::cerr << "Exception in Box2DPhysicsEngine constructor: " << e.what() << std::endl;
            throw;
        }
        
        std::cout << "========== PHYSICS ENGINE INITIALIZATION COMPLETE ==========" << std::endl;
    }

    void Box2DPhysicsEngine::initializeComponents() {
        if (!p_Micras) {
            std::cerr << "Cannot initialize components: Micras body is null" << std::endl;
            return;
        }
        
        // Initialize wall sensors
        micras::proxy::TWallSensors<4>::Config wall_sensor_config;
        wall_sensor_config.uncertainty = 0.01f;
        wall_sensor_config.wall_threshold.fill(0.5f);
        wall_sensor_config.free_threshold.fill(0.1f);
        
        // We can't directly configure the wall sensors after construction
        // They need to be configured during construction, which is handled by Box2DMicrasBody
        
        // Initialize ARGB LEDs
        for (auto& led : p_Micras->getArgb().argbs) {
            led.setColor(micrasverse::types::Colors::red);
        }
    }

    void Box2DPhysicsEngine::update(float deltaTime) {
        if (p_World && p_Micras) {
            p_World->runStep(deltaTime, 1);
            p_Micras->update(deltaTime);
        }
    }

    void Box2DPhysicsEngine::loadMaze(const std::string& mazePath) {
        if (!p_Maze || !p_Micras) {
            std::cerr << "Cannot load maze: Maze or Micras body is null" << std::endl;
            return;
        }
        
        p_Maze->reloadFromFile(mazePath);
        
        // Update the wall sensors to detect the new maze
        for (auto& lidar : p_Micras->getWallSensors().get_sensors()) {
            lidar.update();
        }
    }

    void Box2DPhysicsEngine::resetMicrasPosition() {
        if (!p_Micras) {
            std::cerr << "Cannot reset Micras position: Micras body is null" << std::endl;
            return;
        }
        
        b2BodyId bodyId = p_Micras->getBodyId();
        if (b2Body_IsValid(bodyId)) {
            b2Body_SetTransform(bodyId, (b2Vec2){(CELL_SIZE+WALL_THICKNESS)/2.0f, (CELL_SIZE+WALL_THICKNESS)/2.0f}, (b2Rot){1.0f, 0.0f});
            b2Body_SetLinearVelocity(bodyId, (b2Vec2){0.0f, 0.0f});
            b2Body_SetAngularVelocity(bodyId, 0.0f);
        }
        
        // Reset the ARGB LEDs
        for (auto& argb : p_Micras->getArgb().argbs) {
            argb.setColor(micrasverse::types::Colors::red);
        }
    }

    // Destructor to ensure proper cleanup
    Box2DPhysicsEngine::~Box2DPhysicsEngine() {
        std::cout << "Box2DPhysicsEngine destructor called - destroying physics world" << std::endl;
        // Destroy components in reverse order of creation
        p_Micras.reset();
        p_Maze.reset();
        p_World.reset(); // This will destroy the Box2D world
    }
}
#endif // BOX2D_PHYSICS_ENGINE_CPP
