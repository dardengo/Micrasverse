#ifndef BOX2D_LED_HPP
#define BOX2D_LED_HPP

#include "box2d/box2d.h"
#include "micrasverse_core/types.hpp"

namespace micrasverse::physics {

class Box2DLED {
public:
    Box2DLED(b2BodyId bodyId, const types::Vec2& localPosition, const types::Vec2& size, const types::Color& color);
    virtual ~Box2DLED() = default;

    types::Vec2 getPosition() const;

    void setCommand(float command);  // Not used for LEDs

    float getCommand() const { return 0.0f; }

    bool isActive() const { return isLedOn; }

    void update(float deltaTime);

    // ILED interface implementation
    void         setColor(const types::Color& color);
    types::Color getColor() const;
    void         turnOn();
    void         turnOn(const types::Color& color);
    void         turnOff();
    void         toggle();
    bool         isOn() const;
    types::Vec2  getSize() const;

    // Box2D specific getters for rendering
    b2Vec2                      getWorldPosition() const;
    b2Vec2                      getBox2DSize() const;
    const std::array<float, 3>& getBaseColorArray() const;
    const std::array<float, 3>& getLightColorArray() const;

private:
    b2BodyId             bodyId;
    b2Vec2               localPosition;
    b2Vec2               worldPosition;
    b2Vec2               size;
    bool                 isLedOn;
    types::Color         baseColor;
    types::Color         lightColor;
    std::array<float, 3> baseColorArray;
    std::array<float, 3> lightColorArray;
};

}  // namespace micrasverse::physics

#endif  // BOX2D_LED_HPP
