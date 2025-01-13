#include "physics/motor.h"

namespace micrasverse::physics {

Motor::Motor(b2BodyId bodyId, b2Vec2 localPosition, bool leftWheel, const float angle, float R, float ke, float kt, float maxVoltage)
    : bodyId(bodyId), localPosition(localPosition), leftWheel(leftWheel), angle(angle), resistance(R), ke(ke), kt(kt),
    maxVoltage(maxVoltage), inputCommand(0), current(0), angularVelocity(0) {
    
    localDirection = {cos(angle), sin(angle)};
    bodyMass = MICRAS_MASS;
    frictionCoefficient = MICRAS_FRICTION;
}

void Motor::setCommand(float command) {
    this->inputCommand = std::clamp(command, -100.0f, 100.0f);
}

void Motor::update() {
    // Calculate input voltage based on the command
    float inputVoltage = this->maxVoltage * (this->inputCommand / 100.0f);

    // Compute back EMF
    float angularVelocitySign = (this->leftWheel ? -1.0f : +1.0f);
    float linearVelocityDirection = b2Dot(b2Body_GetLinearVelocity(this->bodyId), b2Body_GetWorldVector(this->bodyId, this->localDirection));
    float wheelAngularVelocity = - std::copysign(
        (2.0f * b2Length(b2Body_GetLinearVelocity(this->bodyId)) + angularVelocitySign * MICRAS_TRACK_WIDTH * b2Body_GetAngularVelocity(this->bodyId))
        / (2.0f * MICRAS_WHEEL_RADIUS),
        linearVelocityDirection
    );
    this->angularVelocity = -wheelAngularVelocity * MICRAS_GEAR_RATIO;
    float backEMF = this->ke * this->angularVelocity;

    // Compute current through the motor
    this->current = (inputVoltage - std::copysign(backEMF, angularVelocity)) / this->resistance;

    // Compute torque
    float torque = kt * this->current;

    // Compute force applied at each wheel (divided by 2 because there are two wheels)
    float force = (torque * MICRAS_GEAR_RATIO) / (MICRAS_WHEEL_RADIUS * 2.0f);

    // Compute maximum frictional force (divided by 4 because there are 4 wheels, and multiplied by 9.81 to convert to N)
    constexpr float maxFrictionForce = MICRAS_FRICTION * MICRAS_MASS * 9.81f / 4.0f;

    // Apply the smaller of motor force and friction force
    float appliedForce = ((std::abs(force) < maxFrictionForce) ? force : std::copysign(maxFrictionForce, force));

    // Apply force at the position of the motor in the direction of angle
    const b2Vec2 worldDirection = b2Body_GetWorldVector(this->bodyId, this->localDirection);

    b2Vec2 forceVector = appliedForce * worldDirection;
    
    b2Body_ApplyForce(this->bodyId, forceVector, b2Body_GetWorldPoint(this->bodyId, this->localPosition), true);
    
    setCommand(std::copysign(0.0f, inputVoltage));
}

float Motor::getCurrent() {
    return this->current;
}

float Motor::getAngularVelocity() {
    return this->angularVelocity;
}

} // namespace micrasverse::physics
