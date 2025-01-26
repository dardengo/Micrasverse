#include "physics/rectanglebody.hpp"
#include "render/keyboard.hpp"
#include "config/constants.hpp"

#include <iostream>

namespace micrasverse::physics {

// Constructor
RectangleBody::RectangleBody(const b2WorldId worldId, const b2Vec2 position, const b2Vec2 size, const b2BodyType type, const float mass, const float restitution, const float friction) {

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

    //Create the body polygon shape    
    b2Polygon shapeBox = b2MakeBox(size.x / 2.0f, size.y / 2.0f);

    //Create the body shape definition
    b2ShapeDef shapeDef = b2DefaultShapeDef();

    shapeDef.friction = friction;
    shapeDef.restitution = restitution;
    shapeDef.density = mass / (size.x * size.y);

    //Create the body shape
    this->shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &shapeBox);
}

// Destructor
RectangleBody::~RectangleBody() {
    bodyId = b2_nullBodyId;
    shapeId = b2_nullShapeId;
}

// Accessor for the Box2D body
b2BodyId RectangleBody::getBodyId() const {
    return bodyId;
}


b2ShapeId RectangleBody::getShapeId() {
    return shapeId;
}

// Optional: Add methods to modify the body
void RectangleBody::setPose(const b2Vec2& position, const b2Rot& rotation) {
    b2Body_SetTransform(bodyId, position, rotation);
}

// ** To-do: pass this to a MicrasBody class
b2Vec2 RectangleBody::getPosition() const {
    return b2Body_GetPosition(bodyId);
}

// ** To-do: pass this to a MicrasBody class
b2Rot RectangleBody::getRotation() const {
    return b2Body_GetRotation(bodyId);
}

b2Vec2 RectangleBody::getSize() const {
    b2Polygon shapeBox = b2Shape_GetPolygon(shapeId);
    return {std::abs(shapeBox.vertices[0].x) * 2.0f, std::abs(shapeBox.vertices[0].y) * 2.0f};
}

} // namespace micrasverse::physics
