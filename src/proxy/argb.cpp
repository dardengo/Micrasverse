#include "micras/proxy/argb.hpp"

namespace micrasverse::proxy {

Argb::Argb(b2BodyId bodyId, b2Vec2 localPosition, b2Vec2 size, glm::vec3 lightColor)
    : bodyId(bodyId), localPosition(localPosition), size(size) {
        this->worldPosition = b2Body_GetWorldPoint(bodyId, localPosition);
        this->lightColor = mapColor(lightColor);
        this->baseColor = mapColor(glm::vec3(176.0f, 198.0f, 214.0f));
        
        this->argbRenderable = render::Rectangle(glm::vec3(worldPosition.x, worldPosition.y, 0.01f), glm::vec3(size.x, size.y, 0.0f), this->isOn ? this->lightColor : this->baseColor);
        this->argbRenderable.init();
    }

void Argb::setColor(const glm::vec3 color){
    this->lightColor = mapColor(color);
    this->argbRenderable.setColor(this->lightColor);
}

void Argb::update(b2Vec2 position, b2Rot rotation){
    this->argbRenderable.setPose(glm::vec3(position.x + this->localPosition.x, position.y + this->localPosition.y, 0.01f), rotation);
}

void Argb::turnOn(){
    this->argbRenderable.setColor(this->lightColor);
    this->isOn = true;
}

void Argb::turnOn(const glm::vec3 color){
    this->lightColor = mapColor(color);
    this->argbRenderable.setColor(this->lightColor);
    this->isOn = true;
}


void Argb::turnOff(){
    this->argbRenderable.setColor(this->baseColor);
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
