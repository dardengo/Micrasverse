#ifndef BOX2D_BUTTON_HPP
#define BOX2D_BUTTON_HPP

#include "box2d/box2d.h"
#include "micrasverse_core/types.hpp"
#include "physics/i_sensor.hpp"
#include "micras/proxy/button.hpp"
#include <chrono>

namespace micrasverse::physics {

class Box2DButton : public ISensor {
public:
    Box2DButton(b2BodyId bodyId, const micrasverse::types::Vec2& localPosition, float radius);
    
    // ISensor interface implementation
    SensorType getType() const override { return SensorType::TOUCH; }
    micrasverse::types::Vec2 getPosition() const override;
    micrasverse::types::Vec2 getDirection() const override { return {0.0f, 1.0f}; } // Button points up
    float getReading() const override { return isPressed() ? 1.0f : 0.0f; }
    void update() override;
    micrasverse::types::Vec2 getRayMidPoint() const override { return getPosition(); }
    micrasverse::types::Vec2 getRayDirection() const override { return getDirection(); }
    float getReadingVisual() const override { return getReading(); }
    
    // Box2DButton specific methods
    bool isPressed() const;
    micras::proxy::Button::Status getStatus() const;

private:
    b2BodyId bodyId;
    b2Vec2 localPosition;
    float radius;
    bool pressed;
    bool previousPressed;
    micras::proxy::Button::Status currentStatus;
    std::chrono::steady_clock::time_point pressStartTime;
    
    static constexpr int SHORT_PRESS_DURATION_MS = 500;
    static constexpr int LONG_PRESS_DURATION_MS = 2000;
};

}  // namespace micrasverse::physics

#endif  // BOX2D_BUTTON_HPP 
