#ifndef BOX2D_MICRASBODY_HPP
#define BOX2D_MICRASBODY_HPP

#include "box2d/box2d.h"
#include "physics/box2d_rectanglebody.hpp"

#include <memory>
#include <vector>

namespace micrasverse::physics {

// Forward declarations for all component types used in this class
class Box2DDistanceSensor;
class Box2DMotor;
class Box2DButton;
class Box2DDipSwitch;
class Box2DLED;
class Argb;

class Box2DMicrasBody {
private:
    // Rectangle body to manage the Box2D body
    std::unique_ptr<RectangleBody> rectBody;
    b2BodyId bodyId;
    
    // Size of the body
    b2Vec2 size;

    // Physical components
    std::vector<std::unique_ptr<Box2DDistanceSensor>> distanceSensors;
    std::unique_ptr<Box2DMotor> leftMotor;
    std::unique_ptr<Box2DMotor> rightMotor;
    std::unique_ptr<Box2DButton> button;
    std::unique_ptr<Box2DDipSwitch> dipSwitch;
    std::unique_ptr<Box2DLED> led;
    std::vector<std::unique_ptr<Argb>> argbs;

    // Acceleration and velocity tracking
    float linearSpeed = 0.0f;
    b2Vec2 acceleration = {0.0f, 0.0f};
    float linearAcceleration = 0.0f;
    
    // Update the body's friction
    void updateFriction();
    
    // Get the lateral velocity
    b2Vec2 getLateralVelocity() const;

public:
    b2Vec2 linearVelocity = {0.0f, 0.0f};
    float getLinearSpeed() const { return linearSpeed; }

    // Constructor
    Box2DMicrasBody(b2WorldId worldId, b2Vec2 position, b2Vec2 size, b2BodyType type, float density, float friction, float restitution);
    
    // Destructor
    ~Box2DMicrasBody();
    
    // Get the body ID
    b2BodyId getBodyId() const { return bodyId; }
    
    // Update the body
    void update(float deltaTime);
    
    // Process input
    void processInput(float deltaTime);
    
    // Get the body's position
    b2Vec2 getPosition() const;
    
    // Get the body's size
    b2Vec2 getSize() const { return size; }
    
    // Get the body's angle
    float getAngle() const;
    
    // Get the body's rotation
    b2Rot getRotation() const { return b2MakeRot(getAngle()); }
    
    // Get the body's acceleration
    float getLinearAcceleration() const { return linearAcceleration; }
    
    // Get the body's linear velocity
    b2Vec2 getLinearVelocity() const { return linearVelocity; }

    // Getters for physical components
    Box2DDistanceSensor& getDistanceSensor(size_t index) { return *distanceSensors[index]; }
    Box2DMotor& getLeftMotor() { return *leftMotor; }
    Box2DMotor& getRightMotor() { return *rightMotor; }
    Box2DButton& getButton() { return *button; }
    Box2DDipSwitch& getDipSwitch() { return *dipSwitch; }
    Box2DLED& getLed() { return *led; }
    Argb& getArgb(size_t index) { return *argbs[index]; }
    std::vector<std::unique_ptr<Argb>>& getArgbs() { return argbs; }
};

} // namespace micrasverse::physics

#endif // BOX2D_MICRASBODY_HPP
