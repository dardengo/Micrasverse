#ifndef BOX2D_PHYSICS_ENGINE_CPP
#define BOX2D_PHYSICS_ENGINE_CPP

#include "physics/box2d_physics_engine.hpp"
#include "physics/box2d_distance_sensor.hpp"
#include "physics/box2d_motor.hpp"
#include "physics/box2d_button.hpp"
#include "physics/box2d_dipswitch.hpp"
#include "physics/box2d_led.hpp"
#include "physics/argb.hpp"
#include "box2d/box2d.h"
#include "micras/proxy/wall_sensors.hpp"
#include "micras/proxy/locomotion.hpp"
#include "micras/proxy/argb.hpp"
#include "micras/proxy/dip_switch.hpp"
#include "constants.hpp"
#include <filesystem>
#include <iostream>

namespace micrasverse::physics {

    Box2DPhysicsEngine::Box2DPhysicsEngine(const std::string_view mazePath) {
        p_World = std::make_unique<World>();
        b2WorldId worldId = p_World->getWorldId();        
        p_Maze = std::make_unique<Maze>(worldId, mazePath); 
        p_Micras = std::make_unique<Box2DMicrasBody>(
            worldId,
            b2Vec2((CELL_SIZE+WALL_THICKNESS)/2.0f, MICRAS_HALFHEIGHT+WALL_THICKNESS),
            b2Vec2(MICRAS_WIDTH, MICRAS_HEIGHT),
            b2_dynamicBody,
            MICRAS_MASS,
            MICRAS_FRICTION,
            MICRAS_RESTITUTION
        );
    }

    void Box2DPhysicsEngine::update(float deltaTime) {
        if (p_World && p_Micras) {
            p_Micras->update(deltaTime);
            p_World->runStep(deltaTime, 1);
        }
    }

    void Box2DPhysicsEngine::loadMaze(const std::string_view mazePath) {      
        p_Maze->reloadFromFile(mazePath);
        for (size_t i = 0; i < 4; i++) {
            p_Micras->getDistanceSensor(i).update();
        }
    }

    void Box2DPhysicsEngine::resetMicrasPosition() {
        b2BodyId bodyId = p_Micras->getBodyId();
        if (b2Body_IsValid(bodyId)) {
            b2Body_SetTransform(bodyId, (b2Vec2){(CELL_SIZE+WALL_THICKNESS)/2.0f, MICRAS_HALFHEIGHT + WALL_THICKNESS}, (b2Rot){1.0f, 0.0f});
            b2Body_SetLinearVelocity(bodyId, (b2Vec2){0.0f, 0.0f});
            b2Body_SetAngularVelocity(bodyId, 0.0f);
        }
        for (size_t i = 0; i < 2; i++) { // Assuming we have 2 argbs
            p_Micras->getArgb(i).update();
        }
        for (size_t i = 0; i < 4; i++) {
            p_Micras->getDistanceSensor(i).update();
        }
    }

    // Destructor to ensure proper cleanup
    Box2DPhysicsEngine::~Box2DPhysicsEngine() {
        std::cout << "Box2DPhysicsEngine destructor called - destroying physics world" << std::endl;
        p_Micras.reset();
        p_Maze.reset();
        p_World.reset();
    }
}
#endif // BOX2D_PHYSICS_ENGINE_CPP
