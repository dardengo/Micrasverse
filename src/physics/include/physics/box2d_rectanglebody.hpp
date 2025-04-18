#ifndef RECTANGLEBODY_HPP
#define RECTANGLEBODY_HPP

#include "box2d/box2d.h"
#include "glm/glm.hpp"

#include <vector>

namespace micrasverse::physics {

class RectangleBody {
protected:
    b2BodyId bodyId;
    b2ShapeId shapeId;  // Rectangle shape

public:
    // Constructor
    RectangleBody(const b2WorldId worldId, const b2Vec2 position, const b2Vec2 size, const b2BodyType type, const float mass, const float restitution, const float friction);

    // Destructor
    virtual ~RectangleBody();

    // Accessor for the Box2D body
    b2BodyId getBodyId() const;

    b2ShapeId getShapeId();

    // Optional: Add methods to modify the body
    void setPose(const b2Vec2& position, const b2Rot& rotation);

    b2Vec2 getPosition() const;

    b2Rot getRotation() const;

    b2Vec2 getSize() const;

};

} // namespace micrasverse::physics

#endif // RECTANGLEBODY_HPP
