#include "physics/argb.hpp"

namespace micrasverse::physics {

Argb::Argb(b2BodyId bodyId, b2Vec2 localPosition, b2Vec2 size, glm::vec3 lightColor)
    : bodyId(bodyId), localPosition(localPosition), size(size) {
        this->worldPosition = b2Body_GetWorldPoint(bodyId, localPosition);
        this->lightColor = mapColor(lightColor);
        this->baseColor = mapColor(glm::vec3(176.0f, 198.0f, 214.0f));
    }

void Argb::setColor(const glm::vec3 color){
    this->lightColor = mapColor(color);
}

void Argb::update(b2Vec2 micrasPosition){
    this->worldPosition = b2Vec2{
        micrasPosition.x + this->localPosition.x,
        micrasPosition.y + this->localPosition.y
    };
}

void Argb::turnOn(){
    this->isOn = true;
}

void Argb::turnOn(const glm::vec3 color){
    this->lightColor = mapColor(color);
    this->isOn = true;
}


void Argb::turnOff(){
    this->isOn = false;
}

void Argb::toggle(){
    if (this->isOn) {
        this->turnOff();
    } else {
        this->turnOn();
    }
}

glm::vec3 Argb::mapColor(glm::vec3 color){
    return color / 255.0f;
}

} // namespace micras::proxy
