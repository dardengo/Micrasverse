#include "physics/argb.hpp"

namespace micrasverse::physics {

Argb::Argb(b2BodyId bodyId, b2Vec2 localPosition, b2Vec2 size, core::Color& lightColor)
    : bodyId(bodyId), localPosition(localPosition), size(size) {
        this->worldPosition = b2Body_GetWorldPoint(bodyId, localPosition);
        this->lightColor = lightColor;
        this->lightColorArray = lightColor.toArray();
        this->baseColor = core::Color(176.0f, 198.0f, 214.0f);
        this->baseColorArray = this->baseColor.toArray();
    }

void Argb::setColor(const core::Color color){
    this->lightColor = color;
    this->lightColorArray = color.toArray();
}

void Argb::update(){
    this->worldPosition = b2Body_GetWorldPoint(this->bodyId, this->localPosition);
}

void Argb::turnOn(){
    this->isOn = true;
}

void Argb::turnOn(const core::Color color){
    this->lightColor = color;
    this->lightColorArray = color.toArray();
    this->isOn = true;
}


void Argb::turnOff(){
    this->isOn = false;
}

void Argb::toggle(){
    this->isOn = !this->isOn;
}

} // namespace micras::proxy
