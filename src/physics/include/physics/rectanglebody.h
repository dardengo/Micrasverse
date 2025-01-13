#ifndef RECTANGLEBODY_H
#define RECTANGLEBODY_H

#include "physics/distancesensor.h"
#include "physics/motor.h"

#include "box2d/box2d.h"
#include "glm/glm.hpp"

#include <vector>

namespace micrasverse::physics {

class RectangleBody {
private:
    b2BodyId bodyId;
    b2ShapeId shapeId;  // Rectangle shape

public:
    std::vector<DistanceSensor> distanceSensors;
    std::vector<Motor> motors;

    // Constructor
    RectangleBody(const b2WorldId worldId, const b2Vec2 position, const b2Vec2 size, const b2BodyType type, const b2MassData mass, const float restitution, const float friction);

    // Destructor
    ~RectangleBody();

    // Accessor for the Box2D body
    b2BodyId getBodyId();

    b2ShapeId getShapeId();

    // Optional: Add methods to modify the body
    void setPose(const b2Vec2& position, const b2Rot& rotation);

    // Process inputs

    void processInput(const float deltaTime);

    b2Vec2 getPosition();

    b2Rot getRotation();

    b2Vec2 getSize();

    b2Vec2 getLateralVelocity();
    
    void updateFriction();

    void attachDistanceSensor(b2Vec2 localPosition, float angle);

    void attachMotor(b2Vec2 localPosition, bool leftWheel);

};

} // namespace micrasverse::physics

#endif // RECTANGLEBODY_H
