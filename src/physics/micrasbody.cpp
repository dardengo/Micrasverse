#include "physics/micrasbody.hpp"
#include "render/keyboard.hpp"
#include "config/target.hpp"

namespace micrasverse::physics {

// Constructor
MicrasBody::MicrasBody(const b2WorldId worldId, const b2Vec2 position, const b2Vec2 size, const b2BodyType type, const float mass, const float restitution, const float friction)
    : RectangleBody(worldId, position, size, type, mass, restitution, friction),
    wallSensors(micrasverse::wall_sensors_config, this->bodyId),
    locomotion(this->bodyId),
    argb(micrasverse::argb_config, this->bodyId)
{

    this->attachDipSwitch(4);

    micrasverse::render::Shader flatColorShader("./render/assets/vertex-core.glsl", "./render/assets/fragment-core.glsl");

    this->shader = flatColorShader;
    
    // Create renderable object
    micrasverse::render::Rectangle micrasRender(
        glm::vec3(position.x , position.y, 0.02f),  // Position of the center
        glm::vec3(size.x, size.y, 0.01f),           // Size
        glm::vec3(15.0f, 125.0f, 15.0f) / 255.0f    // Color (RGB from 0 to 1)
    );

    this->micrasRender = micrasRender;

    // Initialize renderable object
    this->micrasRender.init();
}

MicrasBody::~MicrasBody() {
    this->cleanUp();
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
        argb.update(this->getPosition(), this->getRotation());
    }


    // Update position of micrasRender to match micrasBody
    this->micrasRender.setPose(glm::vec3(this->getPosition().x, this->getPosition().y, micrasRender.position.z), this->getRotation());
}

void MicrasBody::processInput(const float deltaTime) {

    if (render::Keyboard::keyWentDown(GLFW_KEY_SPACE)) {
        this->dipSwitch.toggleSwitch(Switch::FAN);
    }

    if (render::Keyboard::key(GLFW_KEY_W)) {
        this->locomotion.setCommand(100.0f, 0.0f);
    }
    if (render::Keyboard::key(GLFW_KEY_S)) {
        this->locomotion.setCommand(-100.0f, 0.0f);
    }
    if (render::Keyboard::key(GLFW_KEY_A)) {
        this->locomotion.setCommand(0.0f, 2.5f);
    }
    if (render::Keyboard::key(GLFW_KEY_D)) {
        this->locomotion.setCommand(0.0f, -2.5f);
    }

    if (render::Keyboard::keyWentUp(GLFW_KEY_W)) {
        this->locomotion.stop();
    }
    if (render::Keyboard::keyWentUp(GLFW_KEY_S)) {
        this->locomotion.stop();
    }
    if (render::Keyboard::keyWentUp(GLFW_KEY_A)) {
        this->locomotion.stop();
    }
    if (render::Keyboard::keyWentUp(GLFW_KEY_D)) {
        this->locomotion.stop();
    }
}

void MicrasBody::render(const glm::mat4 view, const glm::mat4 projection) {

    this->shader.activate(view, projection);
    
    this->micrasRender.render(this->shader, false);

    for (auto& sensor : this->wallSensors.get_sensors()) {
        sensor.rayRender.render(this->shader, false);
    }

    for (auto& argb : this->argb.argbs) {
        argb.argbRenderable.render(this->shader, false);
    }
}

void MicrasBody::cleanUp() {
    this->micrasRender.cleanUp();
    
    for (auto& sensor : this->wallSensors.get_sensors()) {
        sensor.rayRender.cleanUp();
    }

    for (auto& argb : this->argb.argbs) {
        argb.argbRenderable.cleanUp();
    }
}

} // namespace micrasverse::physics
