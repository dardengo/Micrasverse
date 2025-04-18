#ifndef BOX2D_MICRASBODY_HPP
#define BOX2D_MICRASBODY_HPP

#include "box2d/box2d.h"
#include "micras/proxy/wall_sensors.hpp"
#include "micras/proxy/locomotion.hpp"
#include "micras/proxy/argb.hpp"
#include "micras/proxy/button.hpp"
#include "micras/proxy/dip_switch.hpp"
#include "physics/box2d_rectanglebody.hpp"

#include <memory>

namespace micrasverse::physics {

class Box2DMicrasBody {
private:
    // Rectangle body to manage the Box2D body
    std::unique_ptr<RectangleBody> rectBody;
    
    // Size of the body
    b2Vec2 size;

    // Components as unique_ptrs
    std::unique_ptr<micras::proxy::TWallSensors<4>> wallSensors;
    std::unique_ptr<micras::proxy::Locomotion> locomotion;
    std::unique_ptr<micras::proxy::TArgb<2>> argb;
    std::unique_ptr<micras::proxy::TDipSwitch<4>> dipSwitch;
    std::unique_ptr<micras::proxy::Button> button;

    // Acceleration and velocity tracking
    b2Vec2 linearVelocity = {0.0f, 0.0f};
    b2Vec2 acceleration = {0.0f, 0.0f};
    float linearAcceleration = 0.0f;
    
    // Update the body's friction
    void updateFriction();
    
    // Get the lateral velocity
    b2Vec2 getLateralVelocity() const;

public:
    // Constructor
    Box2DMicrasBody(b2WorldId worldId, b2Vec2 position, b2Vec2 size, b2BodyType type, float density, float friction, float restitution);
    
    // Get the body ID
    b2BodyId getBodyId() const;
    
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
    float getLinearAcceleration() const;
    
    // Get the body's linear velocity
    b2Vec2 getLinearVelocity() const { return linearVelocity; }

    // Attach a DIP switch
    void attachDipSwitch(size_t numSwitches);
    
    // Getters for components
    micras::proxy::TWallSensors<4>& getWallSensors();
    micras::proxy::Locomotion& getLocomotion();
    micras::proxy::TArgb<2>& getArgb();
    micras::proxy::TDipSwitch<4>& getDipSwitch();
    micras::proxy::Button& getButton();
};

} // namespace micrasverse::physics

#endif // BOX2D_MICRASBODY_HPP
