#include "physics/box2d_micrasbody.hpp"
#include "physics/box2d_dipswitch.hpp"
#include "physics/box2d_distance_sensor.hpp"
#include "physics/box2d_motor.hpp"
#include "physics/box2d_rectanglebody.hpp"
#include "physics/box2d_physics_factory.hpp"
#include "constants.hpp"
#include "micras/proxy/wall_sensors.hpp"
#include "micras/proxy/locomotion.hpp"
#include "micras/proxy/argb.hpp"
#include "micras/proxy/button.hpp"
#include "micras/core/types.hpp"
#include "io/keyboard.hpp"
#include "target.hpp"

#include <cstdint>
#include <vector>
#include <cmath>
#include <iostream>

namespace micrasverse::physics {

// Constructor
Box2DMicrasBody::Box2DMicrasBody(b2WorldId worldId, b2Vec2 position, b2Vec2 size, b2BodyType type, float density, float friction, float restitution) 
    : size(size)
    , rectBody(std::make_unique<RectangleBody>(worldId, position, size, type, density, friction, restitution))
{
    std::cout << "========== MICRASBODY CREATION START ==========" << std::endl;
    
    // Store the world ID for validity checking
    if (!b2World_IsValid(worldId)) {
        std::cerr << "ERROR: Invalid world ID in Box2DMicrasBody constructor" << std::endl;
        throw std::runtime_error("Invalid world ID in Box2DMicrasBody constructor");
    }
    
    // Get the body ID from the rectangle body
    b2BodyId bodyId = rectBody->getBodyId();
    
    // Validate the body ID is valid
    if (!b2Body_IsValid(bodyId)) {
        std::cerr << "ERROR: Invalid body ID in Box2DMicrasBody constructor" << std::endl;
        throw std::runtime_error("Invalid body ID in Box2DMicrasBody constructor");
    }
    
    std::cout << "Using body ID from RectangleBody, index: " << bodyId.index1 << std::endl;
    
    // Initialize components with the body ID one at a time
    try {
        // Initialize button first as it seems to be most sensitive to world/body validity
        std::cout << "Initializing button..." << std::endl;
        // Check that worldId is valid before creating button
        if (!b2World_IsValid(worldId)) {
            std::cerr << "ERROR: World ID invalid before button initialization" << std::endl;
            throw std::runtime_error("World ID invalid before button initialization");
        }
        // Validate bodyId again right before button initialization
        if (!b2Body_IsValid(bodyId) || !b2Body_IsEnabled(bodyId)) {
            std::cerr << "ERROR: Body ID invalid or disabled before button initialization" << std::endl;
            throw std::runtime_error("Body ID invalid or disabled before button initialization");
        }
        // Verify that the body's world ID matches our worldId
        b2WorldId bodyWorldId = b2Body_GetWorld(bodyId);
        std::cout << "Body's world ID index: " << bodyWorldId.index1 << ", Original world ID index: " << worldId.index1 << std::endl;
        if (bodyWorldId.index1 != worldId.index1) {
            std::cerr << "ERROR: Body's world ID does not match the provided world ID" << std::endl;
            throw std::runtime_error("Body's world ID mismatch");
        }
        
        // Create a copy of ButtonConfig and set its bodyId
        auto buttonConfig = micras::proxy::ButtonConfig;
        buttonConfig.bodyId = bodyId;
        button = std::make_unique<micras::proxy::Button>(buttonConfig);
        
        std::cout << "Initializing wall sensors..." << std::endl;
        auto wallSensorsConfig = micras::proxy::WallSensorsConfig;
        wallSensorsConfig.bodyId = bodyId;
        wallSensors = std::make_unique<micras::proxy::TWallSensors<4>>(wallSensorsConfig);
        
        std::cout << "Initializing locomotion..." << std::endl;
        auto locomotionConfig = micras::proxy::LocomotionConfig;
        locomotionConfig.bodyId = bodyId;
        locomotion = std::make_unique<micras::proxy::Locomotion>(locomotionConfig);
        
        std::cout << "Initializing ARGB LEDs..." << std::endl;
        auto argbConfig = micras::proxy::ArgbConfig;
        argbConfig.bodyId = bodyId;
        argb = std::make_unique<micras::proxy::TArgb<2>>(argbConfig);
        
        // Attach both LEDs with different positions
        argb->attachArgb(b2Vec2{-micrasverse::MICRAS_HALFWIDTH + 0.01f, 0.0f}, b2Vec2{0.01f, 0.01f}, micrasverse::types::Colors::red);  // Left LED
        argb->attachArgb(b2Vec2{micrasverse::MICRAS_HALFWIDTH - 0.01f, 0.0f}, b2Vec2{0.01f, 0.01f}, micrasverse::types::Colors::green);  // Right LED
        
        std::cout << "Initializing DIP switch..." << std::endl;
        auto dipSwitchConfig = micras::proxy::DipSwitchConfig;
        dipSwitchConfig.bodyId = bodyId;
        dipSwitch = std::make_unique<micras::proxy::TDipSwitch<4>>(dipSwitchConfig);
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR during component initialization: " << e.what() << std::endl;
        throw;
    }
    
    std::cout << "MicrasBody initialization complete" << std::endl;
    std::cout << "========== MICRASBODY CREATION END ==========" << std::endl;
}

b2BodyId Box2DMicrasBody::getBodyId() const {
    if (!rectBody) {
        std::cerr << "ERROR: RectangleBody is null when trying to get bodyId" << std::endl;
        throw std::runtime_error("Invalid RectangleBody in Box2DMicrasBody");
    }
    return rectBody->getBodyId();
}

void Box2DMicrasBody::update(float deltaTime) {
    // Calculate acceleration as change in velocity over time
    b2BodyId bodyId = getBodyId();
    b2Vec2 currentVelocity = b2Body_GetLinearVelocity(bodyId);
    this->acceleration = (currentVelocity - this->linearVelocity) * (1.0f/deltaTime);
    
    // Store current velocity for next calculation
    this->linearVelocity = currentVelocity;

    updateFriction();
    wallSensors->update();
    locomotion->update(deltaTime, false);
    argb->update();
    button->is_pressed();
}

void Box2DMicrasBody::processInput(float deltaTime) {
    if (io::Keyboard::key(GLFW_KEY_SPACE)) {
        locomotion->setCommand(0.5f, 0.0f);
        argb->set_color(micrasverse::types::Colors::red);
    } else {
        locomotion->setCommand(0.0f, 0.0f);
        argb->set_color(micrasverse::types::Colors::green);
    }
}

b2Vec2 Box2DMicrasBody::getPosition() const {
    b2Transform transform = b2Body_GetTransform(getBodyId());
    return transform.p;
}

float Box2DMicrasBody::getAngle() const {
    b2Transform transform = b2Body_GetTransform(getBodyId());
    return b2Rot_GetAngle(transform.q);
}

void Box2DMicrasBody::updateFriction() {
    b2BodyId bodyId = getBodyId();
    b2Vec2 lateralVelocity = getLateralVelocity();
    float mass = b2Body_GetMass(bodyId);
    b2Vec2 position = b2Body_GetWorldCenterOfMass(bodyId);
    b2Vec2 impulse = mass * -lateralVelocity;
    b2Body_ApplyLinearImpulse(bodyId, impulse, position, true);
}

b2Vec2 Box2DMicrasBody::getLateralVelocity() const {
    b2BodyId bodyId = getBodyId();
    b2Vec2 linearVelocity = b2Body_GetLinearVelocity(bodyId);
    b2Transform transform = b2Body_GetTransform(bodyId);
    float angle = b2Rot_GetAngle(transform.q);
    b2Rot rotation = b2MakeRot(angle);
    b2Vec2 lateralNormal = b2RotateVector(rotation, b2Vec2{1.0f, 0.0f});
    return b2Dot(lateralNormal, linearVelocity) * lateralNormal;
}

float Box2DMicrasBody::getLinearAcceleration() const { 
    return this->linearAcceleration; 
}

void Box2DMicrasBody::attachDipSwitch(size_t numSwitches) {
    // Cannot reset with different number of switches since it's template parameter
    // Just log a warning if trying to change the number
    if (numSwitches != 4) {
        std::cerr << "WARNING: Can't change DIP switch count from 4 to " << numSwitches 
                  << " as it's a template parameter" << std::endl;
    }
    
    // Reset with the same template parameter - using make_unique instead of new
    auto dipSwitchConfig = micras::proxy::DipSwitchConfig;
    dipSwitchConfig.bodyId = getBodyId();
    dipSwitch = std::make_unique<micras::proxy::TDipSwitch<4>>(dipSwitchConfig);
}

// Getter implementation
micras::proxy::TWallSensors<4>& Box2DMicrasBody::getWallSensors() {
    return *wallSensors;
}

micras::proxy::Locomotion& Box2DMicrasBody::getLocomotion() {
    return *locomotion;
}

micras::proxy::TArgb<2>& Box2DMicrasBody::getArgb() {
    return *argb;
}

micras::proxy::TDipSwitch<4>& Box2DMicrasBody::getDipSwitch() {
    return *dipSwitch;
}

micras::proxy::Button& Box2DMicrasBody::getButton() {
    return *button;
}

} // namespace micrasverse::physics
