#include "physics/box2d_rectanglebody.hpp"
#include "constants.hpp"
#include <iostream>

namespace micrasverse::physics {

// Constructor
RectangleBody::RectangleBody(const b2WorldId worldId, const b2Vec2 position, const b2Vec2 size, const b2BodyType type, const float mass, const float restitution, const float friction) {
    std::cout << "========== RECTANGLE BODY CREATION START ==========" << std::endl;
    std::cout << "RectangleBody constructor using world ID with index: " << worldId.index1 << std::endl;
    
    // Validate the world ID is valid
    bool isValidWorld = b2World_IsValid(worldId);
    std::cout << "World validity check in RectangleBody: " << (isValidWorld ? "VALID" : "INVALID") << std::endl;
    
    if (!isValidWorld) {
        std::cerr << "ERROR: Invalid world ID passed to RectangleBody constructor" << std::endl;
        throw std::runtime_error("Invalid world ID in RectangleBody constructor");
    }
    
    //Create a the body definition 
    std::cout << "Creating body definition..." << std::endl;
    b2BodyDef bodyDef = b2DefaultBodyDef();
    
    //Set the body position
    bodyDef.position = position;

    //Set the body type
    bodyDef.type = type; 

    std::cout << "Creating Box2D body with world ID index: " << worldId.index1 << std::endl;
    
    //Create the body id — id's are small structures that should be passed by value
    this->bodyId = b2CreateBody(worldId, &bodyDef);
    
    std::cout << "Body created with ID index: " << this->bodyId.index1 << std::endl;
    
    bool isValidBody = b2Body_IsValid(bodyId);
    std::cout << "Body validity check: " << (isValidBody ? "VALID" : "INVALID") << std::endl;
    
    if (!b2Body_IsValid(bodyId)) {
        std::cerr << "ERROR: Failed to create body in RectangleBody constructor" << std::endl;
        throw std::runtime_error("Failed to create body in RectangleBody constructor");
    }

    //Create the body polygon shape    
    b2Polygon boxShape = b2MakeBox(size.x / 2.0f, size.y / 2.0f);
    
    //Create the shape definition
    std::cout << "Creating shape definition..." << std::endl;
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    
    //Set density, as it's the inverse of mass
    //For static bodies set as zero, to make it weightless
    shapeDef.density = (type == b2_staticBody) ? 0.0f : (mass / (size.x * size.y));
    
    //Set friction and restitution
    shapeDef.friction = friction;
    shapeDef.restitution = restitution;
    
    //Create polygon shape to the body
    std::cout << "Creating polygon shape for body..." << std::endl;
    b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &boxShape);
    std::cout << "Shape created with ID index: " << shapeId.index1 << std::endl;
    
    std::cout << "RectangleBody created successfully with position (" << position.x << ", " << position.y 
              << ") and size (" << size.x << ", " << size.y << ")" << std::endl;
    std::cout << "========== RECTANGLE BODY CREATION END ==========" << std::endl;
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
