#include "physics/box2d_rectanglebody.hpp"
#include "constants.hpp"
#include <iostream>

namespace micrasverse::physics {

// Constructor
RectangleBody::RectangleBody(const b2WorldId worldId, const b2Vec2 position, const b2Vec2 size, const b2BodyType type, const float mass, const float restitution, const float friction) {
    // Validate the world ID is valid
    if (!b2World_IsValid(worldId)) {
        throw std::runtime_error("Invalid world ID in RectangleBody constructor");
    }
    
    // Create the body definition
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = position;
    bodyDef.type = type;
    bodyDef.linearDamping = 0.0f;
    bodyDef.angularDamping = 0.0f;
    
    // Create the body
    this->bodyId = b2CreateBody(worldId, &bodyDef);
    
    if (!b2Body_IsValid(bodyId)) {
        throw std::runtime_error("Failed to create body in RectangleBody constructor");
    }

    // Create the body polygon shape    
    b2Polygon boxShape = b2MakeBox(size.x / 2.0f, size.y / 2.0f);
    
    // Create the shape definition
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    
    // Set density, friction and restitution
    shapeDef.density = (type == b2_staticBody) ? 0.0f : (mass / (size.x * size.y));
    
    // Create polygon shape to the body
    this->shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &boxShape);
}

// Destructor
RectangleBody::~RectangleBody() {
    // Clean up the shape and body
    b2DestroyShape(this->shapeId, true);
    b2DestroyBody(this->bodyId);
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
