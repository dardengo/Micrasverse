#include "physics/box2d_motor.hpp"
#include <cmath>

namespace micrasverse::physics {

Box2DMotor::Box2DMotor(b2BodyId bodyId, const core::Vec2& localPosition, bool leftWheel, 
                       float angle, float R, float ke, float kt, float maxVoltage)
    : resistance(R), ke(ke), kt(kt), maxVoltage(maxVoltage), inputCommand(0.0f),
      current(0.0f), angularVelocity(0.0f), frictionCoefficient(0.3f),
      appliedForce(0.0f), torque(0.0f), bodyLinearVelocity(0.0f), bodyAngularVelocity(0.0f),
      leftWheel(leftWheel), isFanOn(false), bodyId(bodyId),
      localPosition{localPosition.x, localPosition.y}, angle(angle) {
    
    // Calculate local direction based on angle
    localDirection.x = std::cos(angle);
    localDirection.y = std::sin(angle);
    
    // Calculate body mass (used for force calculations)
    bodyMass = b2Body_GetMass(bodyId);
}

core::Vec2 Box2DMotor::getPosition() const {
    b2Vec2 worldPos = b2Body_GetWorldPoint(bodyId, localPosition);
    return {worldPos.x, worldPos.y};
}

void Box2DMotor::setCommand(float command) {
    // Clamp command between -100 and 100
    inputCommand = std::max(-100.0f, std::min(100.0f, command));
}

void Box2DMotor::update(float deltaTime) {
    // Skip update if delta time is zero
    if (deltaTime <= 0.0f) return;
    
    // Get body velocities for calculation
    b2Vec2 linearVelocity = b2Body_GetLinearVelocity(bodyId);
    float angVelocity = b2Body_GetAngularVelocity(bodyId);
    
    // Store for getters
    bodyAngularVelocity = angVelocity;
    
    // Calculate body linear velocity in the direction of the motor
    b2Vec2 worldDirection = b2Body_GetWorldVector(bodyId, localDirection);
    bodyLinearVelocity = b2Dot(linearVelocity, worldDirection);
    
    // Calculate voltage from command
    float voltage = (inputCommand / 100.0f) * maxVoltage;
    
    // Calculate motor's angular velocity based on body motion and gear ratio
    // (this is a simplified model)
    float wheelRadius = 0.05f; // 5cm wheel radius
    angularVelocity = bodyLinearVelocity / wheelRadius;
    
    // Calculate back EMF
    float backEMF = ke * angularVelocity;
    
    // Calculate current using V = IR + backEMF
    current = (voltage - backEMF) / resistance;
    
    // Calculate torque: T = kt * I
    torque = kt * current;
    
    // Calculate force: F = T / r
    appliedForce = torque / wheelRadius;
    
    // Apply force to body at motor's position
    b2Vec2 worldPos = b2Body_GetWorldPoint(bodyId, localPosition);
    
    // Create a force vector by multiplying direction by force magnitude
    b2Vec2 forceVec = {
        worldDirection.x * appliedForce,
        worldDirection.y * appliedForce
    };
    
    b2Body_ApplyForce(bodyId, forceVec, worldPos, true);
    
    // Apply fan force if fan is on
    if (isFanOn) {
        // Fan pushes in the opposite direction
        b2Vec2 fanDirection = {-worldDirection.x, -worldDirection.y};
        float fanForce = 0.2f; // Fixed fan force
        
        // Create fan force vector
        b2Vec2 fanForceVec = {
            fanDirection.x * fanForce,
            fanDirection.y * fanForce
        };
        
        b2Body_ApplyForce(bodyId, fanForceVec, worldPos, true);
    }
}

} // namespace micrasverse::physics 
