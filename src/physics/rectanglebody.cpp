#include "physics/rectanglebody.h"
#include "render/keyboard.h"
#include "core/constants.h"

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <iostream>

namespace micrasverse::physics {

// Constructor
RectangleBody::RectangleBody(const b2WorldId worldId, const b2Vec2 position, const b2Vec2 size, const b2BodyType type, const b2MassData mass, const float restitution, const float friction) {

    //Create a the body definition 
    b2BodyDef bodyDef = b2DefaultBodyDef();
    
    //Set the body position
    bodyDef.position = position;

    //Set the body type
    bodyDef.type = type; 

    //Create the body id — id's are small structures that should be passed by value
    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

    //Set the body id
    this->bodyId = bodyId;

    if (bodyDef.type != b2_staticBody) {
        //Set the body mass
        b2Body_SetMassData(bodyId, mass);
    }
    
    //Create the body polygon shape    
    b2Polygon shapeBox = b2MakeBox(size.x / 2.0f, size.y / 2.0f);

    //Create the body shape definition
    b2ShapeDef shapeDef = b2DefaultShapeDef();

    shapeDef.friction = friction;
    shapeDef.restitution = restitution;

    //Create the body shape
    this->shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &shapeBox);
}

// Destructor
RectangleBody::~RectangleBody() {
    bodyId = b2_nullBodyId;
    shapeId = b2_nullShapeId;
}

// Accessor for the Box2D body
b2BodyId RectangleBody::getBodyId() {
    return bodyId;
}


b2ShapeId RectangleBody::getShapeId() {
    return shapeId;
}

// Optional: Add methods to modify the body
void RectangleBody::setPose(const b2Vec2& position, const b2Rot& rotation) {
    b2Body_SetTransform(bodyId, position, rotation);
}

void RectangleBody::processInput(const float deltaTime) {

    b2Vec2 frontNormal = b2Normalize({cos(b2Rot_GetAngle(b2Body_GetRotation(bodyId))+ B2_PI / 2.0f), sin(b2Rot_GetAngle(b2Body_GetRotation(bodyId))+ B2_PI / 2.0f)});

    if (render::Keyboard::key(GLFW_KEY_W)) {
        for (auto& motor : motors) {
            motor.setCommand(10.0f);
        }
    }
    if (render::Keyboard::key(GLFW_KEY_S)) {
        for (auto& motor : motors) {
            motor.setCommand(-10.0f);
        }
    }
    if (render::Keyboard::key(GLFW_KEY_A)) {
        for (auto& motor : motors) {
            if (motor.leftWheel) {
                motor.setCommand(-2.5f);
            } else {
                motor.setCommand( 2.5f);
            }
        }
    }
    if (render::Keyboard::key(GLFW_KEY_D)) {
        for (auto& motor : motors) {
            if (motor.leftWheel) {
                motor.setCommand( 2.5f);
            } else {
                motor.setCommand(-2.5f);
            }
        }
    }

    if (render::Keyboard::keyWentUp(GLFW_KEY_W)) {
        for (auto& motor : motors) {
            motor.setCommand( 0.0f);
        }
    }
    if (render::Keyboard::keyWentUp(GLFW_KEY_S)) {
        for (auto& motor : motors) {
            motor.setCommand( 0.0f);
        }
    }
    if (render::Keyboard::keyWentUp(GLFW_KEY_A)) {
        for (auto& motor : motors) {
            motor.setCommand( 0.0f);
        }
    }
    if (render::Keyboard::keyWentUp(GLFW_KEY_D)) {
        for (auto& motor : motors) {
            motor.setCommand( 0.0f);
        }
    }
}

b2Vec2 RectangleBody::getPosition() {
    return b2Body_GetPosition(bodyId);
}

b2Rot RectangleBody::getRotation() {
    return b2Body_GetRotation(bodyId);
}

b2Vec2 RectangleBody::getSize() {
    b2Polygon shapeBox = b2Shape_GetPolygon(shapeId);
    return {std::abs(shapeBox.vertices[0].x) * 2.0f, std::abs(shapeBox.vertices[0].y) * 2.0f};
}

b2Vec2 RectangleBody::getLateralVelocity(){
    b2Vec2 currentRightNormal = b2Body_GetWorldVector(this->bodyId, {1, 0});
    return b2Dot(currentRightNormal, b2Body_GetLinearVelocity(bodyId)) * currentRightNormal;
}

// ** To-do: pass this to a Wheel class
void RectangleBody::updateFriction(){
    
    // Kill lateral velocity
    b2Vec2 impulse = b2Body_GetMass(bodyId) * -getLateralVelocity();
    b2Body_ApplyLinearImpulse(bodyId, impulse, b2Body_GetWorldCenterOfMass(bodyId), true);

    // Apply angular damping
    b2Body_ApplyAngularImpulse(bodyId, MICRAS_MASS * b2Body_GetRotationalInertia(bodyId) * -b2Body_GetAngularVelocity(bodyId), true);

    // Apply drag
    b2Vec2 currentVelocity = b2Body_GetLinearVelocity(bodyId);
    float currentSpeed = b2Length(currentVelocity);
    float dragForceMagnitude = 0.5f * 1.225f * 0.47f * currentSpeed * currentSpeed; // 0.5 * air density * drag coefficient * speed^2
    b2Body_ApplyForceToCenter(bodyId, -dragForceMagnitude * currentVelocity, true);
}

void RectangleBody::attachDistanceSensor(b2Vec2 localPosition, float angle){
    DistanceSensor sensor = DistanceSensor(this->bodyId, localPosition, angle);
    distanceSensors.push_back(sensor);
}

void RectangleBody::attachMotor(b2Vec2 localPosition, bool leftWheel){
    Motor motor = Motor(this->bodyId, localPosition, leftWheel);
    motors.push_back(motor);
}

} // namespace micrasverse::physics
