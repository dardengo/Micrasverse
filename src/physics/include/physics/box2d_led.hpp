#ifndef BOX2D_LED_HPP
#define BOX2D_LED_HPP

#include "physics/i_actuator.hpp"
#include "physics/i_led.hpp"
#include "box2d/box2d.h"
#include "micrasverse_core/types.hpp"
namespace micrasverse::physics {

class Box2DLED : public ILED {
public:
    Box2DLED(b2BodyId bodyId, const types::Vec2& localPosition, const types::Vec2& size, const types::Color& color);
    virtual ~Box2DLED() = default;

    // IActuator interface implementation
    ActuatorType getType() const override { return ActuatorType::LED; }
    types::Vec2 getPosition() const override;
    
    void setCommand(float command) override; // Not used for LEDs
    float getCommand() const override { return 0.0f; }
    
    bool isActive() const override { return isLedOn; }
    void update(float deltaTime) override;
    
    // ILED interface implementation
    void setColor(const types::Color& color) override;
    types::Color getColor() const override;
    void turnOn() override;
    void turnOn(const types::Color& color) override;
    void turnOff() override;
    void toggle() override;
    bool isOn() const override;
    types::Vec2 getSize() const override;
    
    // Box2D specific getters for rendering
    b2Vec2 getWorldPosition() const;
    b2Vec2 getBox2DSize() const;
    const std::array<float, 3>& getBaseColorArray() const;
    const std::array<float, 3>& getLightColorArray() const;

private:
    b2BodyId bodyId;
    b2Vec2 localPosition;
    b2Vec2 worldPosition;
    b2Vec2 size;
    bool isLedOn;
    types::Color baseColor;
    types::Color lightColor;
    std::array<float, 3> baseColorArray;
    std::array<float, 3> lightColorArray;
};

} // namespace micrasverse::physics

#endif // BOX2D_LED_HPP 
