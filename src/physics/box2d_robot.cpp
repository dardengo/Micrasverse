#include "physics/box2d_robot.hpp"
#include "physics/box2d_distance_sensor.hpp"
#include "physics/box2d_motor.hpp"
#include "physics/box2d_led.hpp"
#include "physics/i_robot_controller.hpp"
#include "physics/i_sensor.hpp"
#include "constants.hpp"
#include <cmath>

namespace micrasverse::physics {

Box2DRobot::Box2DRobot(b2WorldId worldId, const types::Vec2& position, const types::Vec2& size)
    : size(size), linearAcceleration(0.0f), controller(nullptr) {
    
    // Create the Box2D body
    createBody(worldId, position);
    
    // Set up robot hardware components
    setupDistanceSensors();
    setupMotors();
    setupLEDs();
}

Box2DRobot::~Box2DRobot() {
    // Box2D cleanup will be handled by the world
}

types::Vec2 Box2DRobot::getPosition() const {
    b2Vec2 position = b2Body_GetPosition(bodyId);
    return {position.x, position.y};
}

float Box2DRobot::getRotation() const {
    // Get the rotation from the transform
    b2Transform transform = b2Body_GetTransform(bodyId);
    return b2Rot_GetAngle(transform.q);
}

types::Vec2 Box2DRobot::getSize() const {
    return size;
}

float Box2DRobot::getLinearVelocity() const {
    b2Vec2 velocity = b2Body_GetLinearVelocity(bodyId);
    return std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
}

float Box2DRobot::getAngularVelocity() const {
    return b2Body_GetAngularVelocity(bodyId);
}

float Box2DRobot::getLinearAcceleration() const {
    return linearAcceleration;
}

const std::vector<std::shared_ptr<ISensor>>& Box2DRobot::getSensors() const {
    return sensors;
}

const std::vector<std::shared_ptr<IActuator>>& Box2DRobot::getActuators() const {
    return actuators;
}

IRobotController* Box2DRobot::getController() {
    return controller.get();
}

void Box2DRobot::setController(std::shared_ptr<IRobotController> controller) {
    this->controller = controller;
    if (this->controller) {
        this->controller->setRobot(this);
        this->controller->init();
    }
}

void Box2DRobot::update(float deltaTime) {
    // Calculate linear acceleration
    static b2Vec2 prevVelocity = b2Body_GetLinearVelocity(bodyId);
    b2Vec2 currentVelocity = b2Body_GetLinearVelocity(bodyId);
    
    // Calculate acceleration manually
    b2Vec2 acceleration;
    acceleration.x = (currentVelocity.x - prevVelocity.x) / deltaTime;
    acceleration.y = (currentVelocity.y - prevVelocity.y) / deltaTime;
    linearAcceleration = std::sqrt(acceleration.x * acceleration.x + acceleration.y * acceleration.y);
    prevVelocity = currentVelocity;
    
    // Update all sensors
    for (auto& sensor : sensors) {
        sensor->update();
    }
    
    // Run controller
    if (controller) {
        controller->update(deltaTime);
    }
    
    // Update all actuators
    for (auto& actuator : actuators) {
        actuator->update(deltaTime);
    }
}

void Box2DRobot::reset() {
    // Create a zero rotation
    b2Rot zeroRot = {1.0f, 0.0f}; // cos=1, sin=0 for angle 0
    
    // Reset position and rotation to zero
    b2Body_SetTransform(bodyId, {0.0f, 0.0f}, zeroRot);
    
    // Reset velocities
    b2Body_SetLinearVelocity(bodyId, {0.0f, 0.0f});
    b2Body_SetAngularVelocity(bodyId, 0.0f);
    
    // Reset controller
    if (controller) {
        controller->reset();
    }
}

b2BodyId Box2DRobot::getBodyId() const {
    return bodyId;
}

void Box2DRobot::createBody(b2WorldId worldId, const types::Vec2& position) {
    // Create body definition using Box2D API
    b2BodyDef bodyDef = b2DefaultBodyDef();
    
    // Set the body position and type
    bodyDef.position = {position.x, position.y};
    bodyDef.type = b2_dynamicBody;
    bodyDef.linearDamping = 0.75f;
    bodyDef.angularDamping = 0.75f;
    
    // Create the body
    bodyId = b2CreateBody(worldId, &bodyDef);
    
    // Create polygon shape for the robot body
    b2Polygon boxShape = b2MakeBox(size.x * 0.5f, size.y * 0.5f);
    
    // Create shape definition
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    
    // Create shape and attach to body
    b2CreatePolygonShape(bodyId, &shapeDef, &boxShape);
}

void Box2DRobot::setupDistanceSensors() {
    // Create 4 distance sensors at the specified positions and angles
    const float sensorMaxDistance = 3.0f;
    
    // Front-right sensor
    auto frontRightSensor = std::make_shared<Box2DDistanceSensor>(
        b2Body_GetWorld(bodyId),
        bodyId,
        types::Vec2{ 0.028f, 0.045f},
        types::Vec2{std::cos(B2_PI / 2.0f), std::sin(B2_PI / 2.0f)},
        sensorMaxDistance
    );
    sensors.push_back(frontRightSensor);
    
    // Front-left sensor
    auto frontLeftSensor = std::make_shared<Box2DDistanceSensor>(
        b2Body_GetWorld(bodyId),
        bodyId,
        types::Vec2{-0.028f, 0.045f},
        types::Vec2{std::cos(B2_PI / 2.0f), std::sin(B2_PI / 2.0f)},
        sensorMaxDistance
    );
    sensors.push_back(frontLeftSensor);
    
    // Front-right diagonal sensor
    auto frontRightDiagonal = std::make_shared<Box2DDistanceSensor>(
        b2Body_GetWorld(bodyId),
        bodyId,
        types::Vec2{ 0.009f, 0.049f},
        types::Vec2{std::cos(B2_PI / 6.0f), std::sin(B2_PI / 6.0f)},
        sensorMaxDistance
    );
    sensors.push_back(frontRightDiagonal);
    
    // Front-left diagonal sensor
    auto frontLeftDiagonal = std::make_shared<Box2DDistanceSensor>(
        b2Body_GetWorld(bodyId),
        bodyId,
        types::Vec2{-0.009f, 0.049f},
        types::Vec2{std::cos(5.0f * B2_PI / 6.0f), std::sin(5.0f * B2_PI / 6.0f)},
        sensorMaxDistance
    );
    sensors.push_back(frontLeftDiagonal);
}

void Box2DRobot::setupMotors() {
    // Create left and right motors
    auto leftMotor = std::make_shared<Box2DMotor>(
        bodyId,
        types::Vec2{0.0f, size.y * 0.5f},
        true
    );
    actuators.push_back(leftMotor);
    
    auto rightMotor = std::make_shared<Box2DMotor>(
        bodyId,
        types::Vec2{0.0f, -size.y * 0.5f},
        false
    );
    actuators.push_back(rightMotor);
}

void Box2DRobot::setupLEDs() {
    // Create RGB LEDs
    const float ledSize = 0.01f;  // 1cm x 1cm
    
    // Front LED
    auto frontLED = std::make_shared<Box2DLED>(
        bodyId,
        types::Vec2{size.x * 0.5f, 0.0f},
        types::Vec2{ledSize, ledSize},
        types::Color(0.0f, 1.0f, 0.0f)  // Green by default
    );
    actuators.push_back(frontLED);
    
    // Back LED
    auto backLED = std::make_shared<Box2DLED>(
        bodyId,
        types::Vec2{-size.x * 0.5f, 0.0f},
        types::Vec2{ledSize, ledSize},
        types::Color(1.0f, 0.0f, 0.0f)  // Red by default
    );
    actuators.push_back(backLED);
}

} // namespace micrasverse::physics 
