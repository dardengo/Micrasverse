#include "physics/argb.hpp"

namespace micrasverse::physics {

Argb::Argb(b2BodyId bodyId, b2Vec2 localPosition, b2Vec2 size, const types::Color& lightColor)
    : bodyId(bodyId)
    , localPosition(localPosition)
    , size(size)
    , lightColor(lightColor)
    , baseColor(176.0f, 198.0f, 214.0f)
    , isLedOn(false)
    , command(0.0f) {
    if (!b2Body_IsValid(bodyId)) {
        throw std::runtime_error("Invalid body ID in Argb constructor");
    }
    this->worldPosition = b2Body_GetWorldPoint(bodyId, localPosition);
    this->lightColorArray = lightColor.toArray();
    this->baseColorArray = this->baseColor.toArray();
}

void Argb::setColor(const types::Color& color){
    this->lightColor = color;
    this->lightColorArray = color.toArray();
}

types::Color Argb::getColor() const {
    return this->lightColor;
}

void Argb::update(){
    this->worldPosition = b2Body_GetWorldPoint(this->bodyId, this->localPosition);
}

void Argb::turnOn(){
    this->isLedOn = true;
}

void Argb::turnOn(const types::Color& color){
    this->lightColor = color;
    this->lightColorArray = color.toArray();
    this->isLedOn = true;
}

void Argb::turnOff(){
    this->isLedOn = false;
}

void Argb::toggle(){
    this->isLedOn = !this->isLedOn;
}

bool Argb::isOn() const {
    return this->isLedOn;
}

types::Vec2 Argb::getWorldPosition() const {
    return {this->worldPosition.x, this->worldPosition.y};
}

types::Vec2 Argb::getSize() const {
    return {this->size.x, this->size.y};
}

// IActuator methods
types::Vec2 Argb::getPosition() const {
    return types::Vec2{this->worldPosition.x, this->worldPosition.y};
}

void Argb::setCommand(float command) {
    this->command = command;
    // For LEDs, command can be used to control brightness
    if (command > 0) {
        this->turnOn();
    } else {
        this->turnOff();
    }
}

float Argb::getCommand() const {
    return this->command;
}

bool Argb::isActive() const {
    return this->isLedOn;
}

void Argb::update(float deltaTime) {
    // Update the world position
    this->update();
}

void Argb::attachArgb(types::Vec2 localPosition, types::Vec2 size, types::Color color) {
    this->localPosition = b2Vec2(localPosition.x, localPosition.y);
    this->size = b2Vec2(size.x, size.y);
    this->lightColor = color;
    this->lightColorArray = color.toArray();
    this->worldPosition = b2Body_GetWorldPoint(this->bodyId, this->localPosition);
}

void Argb::set_color(const types::Color& color, uint8_t index) {
    // For now, just set the main color
    this->lightColor = color;
    this->lightColorArray = color.toArray();
}

void Argb::set_colors(const std::array<types::Color, 8>& colors) {
    // For now, just use the first color
    if (!colors.empty()) {
        this->lightColor = colors[0];
        this->lightColorArray = colors[0].toArray();
    }
}

void Argb::encode_color(const types::Color& color, uint8_t index) {
    // For now, just set the main color
    this->lightColor = color;
    this->lightColorArray = color.toArray();
}

} // namespace micrasverse::physics
