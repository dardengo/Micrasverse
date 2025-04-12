#include "physics/box2d_led.hpp"

namespace micrasverse::physics {

Box2DLED::Box2DLED(b2BodyId bodyId, const core::Vec2& localPosition, const core::Vec2& size, const core::Color& color)
    : bodyId(bodyId), 
      localPosition{localPosition.x, localPosition.y}, 
      size{size.x, size.y},
      isLedOn(false),
      baseColor(core::Color(176.0f, 198.0f, 214.0f)), // Light gray base color
      lightColor(color) {
    
    // Initialize world position
    worldPosition = b2Body_GetWorldPoint(bodyId, this->localPosition);
    
    // Set up color arrays for rendering
    baseColorArray = baseColor.toArray();
    lightColorArray = lightColor.toArray();
}

core::Vec2 Box2DLED::getPosition() const {
    return {worldPosition.x, worldPosition.y};
}

void Box2DLED::setCommand(float command) {
    // LEDs don't use typical commands, but we could interpret a command
    // as a brightness level for future enhancement
    if (command > 0.1f) {
        turnOn();
    } else {
        turnOff();
    }
}

void Box2DLED::update(float deltaTime) {
    // Update world position based on body movement
    worldPosition = b2Body_GetWorldPoint(bodyId, localPosition);
}

void Box2DLED::setColor(const core::Color& color) {
    lightColor = color;
    lightColorArray = color.toArray();
}

core::Color Box2DLED::getColor() const {
    return lightColor;
}

void Box2DLED::turnOn() {
    isLedOn = true;
}

void Box2DLED::turnOn(const core::Color& color) {
    setColor(color);
    turnOn();
}

void Box2DLED::turnOff() {
    isLedOn = false;
}

void Box2DLED::toggle() {
    isLedOn = !isLedOn;
}

bool Box2DLED::isOn() const {
    return isLedOn;
}

core::Vec2 Box2DLED::getSize() const {
    return {size.x, size.y};
}

b2Vec2 Box2DLED::getWorldPosition() const {
    return worldPosition;
}

b2Vec2 Box2DLED::getBox2DSize() const {
    return size;
}

const std::array<float, 3>& Box2DLED::getBaseColorArray() const {
    return baseColorArray;
}

const std::array<float, 3>& Box2DLED::getLightColorArray() const {
    return lightColorArray;
}

} // namespace micrasverse::physics 
