#include "physics/box2d_motor.hpp"
#include <cmath>

namespace micrasverse::physics {

Box2DMotor::Box2DMotor(b2BodyId bodyId, const types::Vec2& localPosition, bool leftWheel, 
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

types::Vec2 Box2DMotor::getPosition() const {
    b2Vec2 worldPos = b2Body_GetWorldPoint(bodyId, localPosition);
    return {worldPos.x, worldPos.y};
}

void Box2DMotor::setCommand(float command) {
    // Clamp command between -100 and 100
    inputCommand = std::max(-100.0f, std::min(100.0f, command));
}

void Box2DMotor::update(float deltaTime, bool fanState) {
    // Update fan state
    isFanOn = fanState;
    
    // Check if body is valid before proceeding
    if (!b2Body_IsValid(bodyId)) {
        // Reset values if body is invalid
        current = 0.0f;
        angularVelocity = 0.0f;
        appliedForce = 0.0f;
        torque = 0.0f;
        bodyLinearVelocity = 0.0f;
        bodyAngularVelocity = 0.0f;
        return;
    }
    
    // If command is near zero, just return without applying force
    if (std::abs(inputCommand) < 0.1f) {
        appliedForce = 0.0f;
        torque = 0.0f;
        return;
    }
    
    // Get body transform to convert local to world coordinates
    b2Transform transform = b2Body_GetTransform(bodyId);
    
    // Calculate voltage based on command (-100 to +100 mapped to -maxVoltage to +maxVoltage)
    float voltage = (inputCommand / 100.0f) * maxVoltage;
    
    // Get current body velocity
    b2Vec2 linearVelocity = b2Body_GetLinearVelocity(bodyId);
    bodyAngularVelocity = b2Body_GetAngularVelocity(bodyId);
    
    // Calculate world direction of motor
    b2Vec2 worldDirection = b2RotateVector(transform.q, localDirection);
    
    // Calculate the linear velocity at the wheel contact point
    b2Vec2 worldPosition = b2TransformPoint(transform, localPosition);
    b2Vec2 r = b2Sub(worldPosition, b2Body_GetWorldCenterOfMass(bodyId));
    b2Vec2 tangentialVel = b2CrossSV(bodyAngularVelocity, r);
    b2Vec2 pointVelocity = b2Add(linearVelocity, tangentialVel);
    
    // Project the velocity onto the wheel direction
    bodyLinearVelocity = b2Dot(pointVelocity, worldDirection);
    
    // Calculate angular velocity of the wheel based on body's linear velocity
    // This is a simplification; in reality you'd need to account for the wheel's radius
    angularVelocity = bodyLinearVelocity / 0.01f; // assuming wheel radius 0.01
    
    // Motor modeling:
    // V = IR + Ke*ω
    // I = (V - Ke*ω) / R
    // T = Kt*I
    // F = T / radius
    
    // Reduce angular velocity if fan is on (simulating power draw)
    if (isFanOn) {
        voltage *= 0.9f; // Reduce voltage by 10% when fan is on
    }
    
    // Back EMF calculation
    float backEMF = ke * angularVelocity;
    
    // Motor current calculation
    current = (voltage - backEMF) / resistance;
    
    // Generate motor torque
    torque = kt * current;
    
    // Convert torque to force (assuming wheel radius of 0.01)
    // F = T / r
    appliedForce = torque / 0.01f;
    
    // Apply the force to the body
    b2Body_ApplyForce(bodyId, b2MulSV(appliedForce, worldDirection), worldPosition, true);
}

} // namespace micrasverse::physics 
