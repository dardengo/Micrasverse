#ifndef MICRAS_BODY_HPP
#define MICRAS_BODY_HPP

#include "physics/box2d_rectanglebody.hpp"
#include "physics/distancesensor.hpp"
#include "physics/motor.hpp"
#include "physics/dipswitch.hpp"
#include "physics/argb.hpp"
#include "config/constants.hpp"
#include "proxy/wall_sensors.hpp"
#include "proxy/locomotion.hpp"
#include "proxy/argb.hpp"

#include "box2d/box2d.h"

#include <cstdint>
#include <vector>

namespace micrasverse::physics {

class MicrasBody : public RectangleBody {
public:
    DipSwitch dipSwitch;
    micras::proxy::TWallSensors<4> wallSensors;
    micras::proxy::Locomotion locomotion;
    micras::proxy::TArgb<1> argb;
    b2Vec2 linearVelocity;
    b2Vec2 acceleration;
    float linearAcceleration;

    enum Switch : uint8_t {
        DIAGONAL = 0,  // Whether the robot will be able to move in diagonal paths.
        FAN = 1,       // Turn the fan on.
        STOP = 2,      // Whether the robot will stop at each intersection when solving the maze.
        TURBO = 3,     // Increase the robot speed.
    };

    // Constructor
    MicrasBody(const b2WorldId worldId,
               const b2Vec2 position = (b2Vec2){(CELL_SIZE+WALL_THICKNESS)/2.0f, (CELL_SIZE+WALL_THICKNESS)/2.0f},
               const b2Vec2 size = (b2Vec2){MICRAS_WIDTH, MICRAS_HEIGHT},
               const b2BodyType type = b2_dynamicBody,
               const float mass = MICRAS_MASS,
               const float restitution = MICRAS_RESTITUTION,
               const float friction = MICRAS_FRICTION
    );

    b2Vec2 getLateralVelocity() const;

    void updateFriction();

    void attachDipSwitch(size_t numSwitches);

    void update(const float deltaTime);

    void processInput(const float deltaTime);
};

} // namespace micrasverse::physics

#endif // MICRAS_BODY_HPP
