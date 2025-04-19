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
    // Get the body ID from the rectangle body
    b2BodyId bodyId = rectBody->getBodyId();
    
    // Validate the body ID is valid
    if (!b2Body_IsValid(bodyId)) {
        throw std::runtime_error("Invalid body ID in Box2DMicrasBody constructor");
    }
    
    // Initialize components with the body ID
    try {
        // Initialize button
        auto buttonConfig = micras::button_config;
        buttonConfig.bodyId = bodyId;
        buttonConfig.worldId = worldId;
        button = std::make_unique<micras::proxy::Button>(buttonConfig);
        
        // Initialize wall sensors
        auto wallSensorsConfig = micras::wall_sensors_config;
        wallSensorsConfig.bodyId = bodyId;
        wallSensors = std::make_unique<micras::proxy::TWallSensors<4>>(wallSensorsConfig);
        
        // Initialize locomotion
        auto locomotionConfig = micras::locomotion_config;
        locomotionConfig.bodyId = bodyId;
        locomotion = std::make_unique<micras::proxy::Locomotion>(locomotionConfig);
        
        // Initialize ARGB LEDs
        auto argbConfig = micras::argb_config;
        argbConfig.bodyId = bodyId;
        argb = std::make_unique<micras::proxy::TArgb<2>>(argbConfig);
        
        // Attach both LEDs with different positions
        argb->attachArgb(b2Vec2{-micrasverse::MICRAS_HALFWIDTH + 0.02f, 0.02f}, b2Vec2{0.01f, 0.01f}, micrasverse::types::Colors::red);  // Left LED
        argb->attachArgb(b2Vec2{micrasverse::MICRAS_HALFWIDTH - 0.02f, 0.02f}, b2Vec2{0.01f, 0.01f}, micrasverse::types::Colors::green);  // Right LED
        
        // Initialize DIP switch
        auto dipSwitchConfig = micras::dip_switch_config;
        dipSwitchConfig.bodyId = bodyId;
        dipSwitch = std::make_unique<micras::proxy::TDipSwitch<4>>(dipSwitchConfig);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Error during component initialization: " + std::string(e.what()));
    }
}

b2BodyId Box2DMicrasBody::getBodyId() const {
    if (!rectBody) {
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
        locomotion->set_command(0.5f, 0.0f);
        argb->set_color(micrasverse::types::Colors::red);
    } else {
        locomotion->set_command(0.0f, 0.0f);
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
    if (numSwitches != 4) {
        std::cerr << "WARNING: Can't change DIP switch count from 4 to " << numSwitches 
                  << " as it's a template parameter" << std::endl;
    }
    
    // Reset with the same template parameter
    auto dipSwitchConfig = micras::dip_switch_config;
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
