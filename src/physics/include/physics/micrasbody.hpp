#ifndef MICRAS_BODY_HPP
#define MICRAS_BODY_HPP

#include "physics/rectanglebody.hpp"
#include "physics/distancesensor.hpp"
#include "physics/motor.hpp"
#include "physics/dipswitch.hpp"
#include "core/constants.hpp"
#include "models/rectangle.hpp"
#include "render/shader.hpp"

#include "box2d/box2d.h"
#include "glm/glm.hpp"

namespace micrasverse::physics {

class MicrasBody : public RectangleBody {
private:
    render::Rectangle micrasRender;
    render::Shader shader;


public:
    DipSwitch dipSwitch;
    std::vector<DistanceSensor> distanceSensors;
    std::vector<Motor> motors;

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
               const b2MassData mass = (b2MassData){MICRAS_MASS, MICRAS_MASS*(MICRAS_HEIGHT*MICRAS_HEIGHT + MICRAS_WIDTH*MICRAS_WIDTH)/12.0f},
               const float restitution = MICRAS_RESTITUTION,
               const float friction = MICRAS_FRICTION
    );

    ~MicrasBody();

    b2Vec2 getLateralVelocity() const;

    void updateFriction();

    void attachDistanceSensor(b2Vec2 localPosition, float angle);

    void attachMotor(b2Vec2 localPosition, bool leftWheel);

    void attachDipSwitch(size_t numSwitches);
    
    void update(const float deltaTime);

    void processInput(const float deltaTime);

    void render(const glm::mat4 view, const glm::mat4 projection);

    void cleanUp();
};

} // namespace micrasverse::physics

#endif // MICRAS_BODY_HPP
