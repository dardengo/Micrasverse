#include "physics/box2d_motor.hpp"
#include <cmath>

namespace micrasverse::physics {

Box2DMotor::Box2DMotor(b2BodyId bodyId, const types::Vec2& localPosition, bool leftWheel, 
                       float angle, float R, float ke, float kt, float maxVoltage)
    : resistance(R), ke(ke), kt(kt), maxVoltage(maxVoltage), inputCommand(0.0f),
      current(0.0f), angularVelocity(0.0f), frictionCoefficient(MICRAS_FRICTION),
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
    // Calculate input voltage based on the command
    float inputVoltage = this->maxVoltage * (this->inputCommand / 100.0f);

    // Compute back EMF
    float angularVelocitySign = (this->leftWheel ? -1.0f : +1.0f);
    float linearVelocityDirection = b2Dot(b2Body_GetLinearVelocity(this->bodyId), b2Body_GetWorldVector(this->bodyId, this->localDirection));

    this->bodyLinearVelocity = std::copysignf(b2Length(b2Body_GetLinearVelocity(this->bodyId)), linearVelocityDirection);

    this->bodyAngularVelocity = b2Body_GetAngularVelocity(this->bodyId);

    float wheelAngularVelocity = (bodyLinearVelocity+angularVelocitySign*MICRAS_TRACK_WIDTH/2.0f*b2Body_GetAngularVelocity(this->bodyId))/(MICRAS_WHEEL_RADIUS);
    
    this->angularVelocity = wheelAngularVelocity * MICRAS_GEAR_RATIO;

    float backEMF = this->ke * this->angularVelocity;

    // Compute current through the motor
    this->current = (inputVoltage - std::copysignf(backEMF, angularVelocity)) / this->resistance;

    // Compute torque
    this->torque = this->kt * this->current;

    // Compute force applied at each wheel
    float force = (this->torque * MICRAS_GEAR_RATIO) / (MICRAS_WHEEL_RADIUS);

    float fanEffect = isFanOn ? 4.0f : 1.0f;

    // Compute maximum frictional force
    float maxFrictionForce = fanEffect * MICRAS_FRICTION * MICRAS_MASS * 9.81f;

    // Apply the smaller of motor force and friction force
    float appliedForce = ((std::abs(force) < maxFrictionForce) ? force : std::copysign(maxFrictionForce, force));

    this->appliedForce = appliedForce;

    // Apply force at the position of the motor in the direction of angle
    const b2Vec2 worldDirection = b2Body_GetWorldVector(this->bodyId, this->localDirection);

    b2Vec2 forceVector = appliedForce * worldDirection;
    
    b2Body_ApplyForce(this->bodyId, forceVector, b2Body_GetWorldPoint(this->bodyId, this->localPosition), true);
}

} // namespace micrasverse::physics 
