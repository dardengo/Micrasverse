#include "physics/micrasbody.hpp"
#include "io/keyboard.hpp"
#include "config/target.hpp"

namespace micrasverse::physics {

// Constructor
MicrasBody::MicrasBody(const b2WorldId worldId, const b2Vec2 position, const b2Vec2 size, const b2BodyType type, const float mass, const float restitution, const float friction)
    : RectangleBody(worldId, position, size, type, mass, restitution, friction),
    wallSensors(micras::proxy::wall_sensors_config, this->bodyId),
    locomotion(this->bodyId),
    argb(micras::proxy::argb_config, this->bodyId)
{
    this->attachDipSwitch(4);
}

b2Vec2 MicrasBody::getLateralVelocity() const {
    b2Vec2 currentRightNormal = b2Body_GetWorldVector(this->bodyId, {1, 0});
    return b2Dot(currentRightNormal, b2Body_GetLinearVelocity(this->bodyId)) * currentRightNormal;
}

// ** To-do: pass this to a Wheel class
void MicrasBody::updateFriction(){
    
    // Kill lateral velocity
    b2Vec2 impulse = b2Body_GetMass(this->bodyId) * -getLateralVelocity();
    b2Body_ApplyLinearImpulse(this->bodyId, impulse, b2Body_GetWorldCenterOfMass(this->bodyId), true);

    /*
    // Apply angular damping
    b2Body_ApplyAngularImpulse(this->bodyId, MICRAS_MASS * b2Body_GetRotationalInertia(this->bodyId) * -b2Body_GetAngularVelocity(this->bodyId), true);

    // Apply drag
    b2Vec2 currentVelocity = b2Body_GetLinearVelocity(this->bodyId);
    b2Vec2 currentDirection = b2Normalize(currentVelocity);
    float currentSpeed = b2Length(currentVelocity);
    float dragForceMagnitude = 0.5f * 1.225f * 0.47f * currentSpeed * currentSpeed; // 0.5 * air density * drag coefficient * speed^2

    b2Body_ApplyForceToCenter(this->bodyId, -dragForceMagnitude * currentDirection, true);*/
}

void MicrasBody::attachDipSwitch(size_t numSwitches){
    DipSwitch dipSwitch = DipSwitch(numSwitches);
    this->dipSwitch = dipSwitch;
}

void MicrasBody::update(const float deltaTime) {
    // Compute acceleration
    this->acceleration = (b2Body_GetLinearVelocity(this->bodyId) - this->linearVelocity) * (1/deltaTime);
    float linearAccelerationDirection = b2Dot(this->acceleration, b2Body_GetWorldVector(this->bodyId, {0,1}));
    this-> linearAcceleration = std::copysignf(b2Length(this->acceleration), linearAccelerationDirection);

    this->linearVelocity = b2Body_GetLinearVelocity(this->bodyId);

    this->processInput(deltaTime);

    this->updateFriction();
    
    this->locomotion.update(deltaTime, this->dipSwitch.readSwitch(Switch::FAN));

    for (auto& sensor : this->wallSensors.get_sensors()) {
        sensor.getReading();
    }

    for (auto& argb : this->argb.argbs) {
        if (this->dipSwitch.readSwitch(Switch::FAN)) {
            argb.turnOn();
        } else {
            argb.turnOff();
        }
        
        argb.update();
    }
}

void MicrasBody::processInput(const float deltaTime) {

    if (io::Keyboard::keyWentDown(GLFW_KEY_SPACE)) {
        this->dipSwitch.toggleSwitch(Switch::FAN);
    }

    if (io::Keyboard::key(GLFW_KEY_W)) {
        this->locomotion.setCommand(100.0f, 0.0f);
    }
    if (io::Keyboard::key(GLFW_KEY_S)) {
        this->locomotion.setCommand(-100.0f, 0.0f);
    }
    if (io::Keyboard::key(GLFW_KEY_A)) {
        this->locomotion.setCommand(0.0f, 2.5f);
    }
    if (io::Keyboard::key(GLFW_KEY_D)) {
        this->locomotion.setCommand(0.0f, -2.5f);
    }

    if (io::Keyboard::keyWentUp(GLFW_KEY_W)) {
        this->locomotion.stop();
    }
    if (io::Keyboard::keyWentUp(GLFW_KEY_S)) {
        this->locomotion.stop();
    }
    if (io::Keyboard::keyWentUp(GLFW_KEY_A)) {
        this->locomotion.stop();
    }
    if (io::Keyboard::keyWentUp(GLFW_KEY_D)) {
        this->locomotion.stop();
    }
}

} // namespace micrasverse::physics
