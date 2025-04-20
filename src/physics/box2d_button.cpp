#include "physics/box2d_button.hpp"
#include "io/keyboard.hpp"

namespace micrasverse::physics {

Box2DButton::Box2DButton(b2BodyId bodyId, const micrasverse::types::Vec2& localPosition, float radius)
    : bodyId(bodyId)
    , localPosition({localPosition.x, localPosition.y})
    , radius(radius)
    , pressed(false)
    , previousPressed(false)
    , currentStatus(micras::proxy::Button::Status::NO_PRESS)
{
}

void Box2DButton::update() {
    // Store previous state for edge detection
    previousPressed = pressed;
    
    // For now, just use keyboard input for testing
    // In a real implementation, this would check for collisions with other bodies
    pressed = io::Keyboard::key(GLFW_KEY_B);
    
    // Handle press state changes
    if (pressed && !previousPressed) {
        // Button was just pressed
        pressStartTime = std::chrono::steady_clock::now();
        currentStatus = micras::proxy::Button::Status::SHORT_PRESS;
    } else if (!pressed && previousPressed) {
        // Button was just released
        auto now = std::chrono::steady_clock::now();
        auto pressDuration = std::chrono::duration_cast<std::chrono::milliseconds>(now - pressStartTime).count();
        
        if (pressDuration >= LONG_PRESS_DURATION_MS) {
            currentStatus = micras::proxy::Button::Status::EXTRA_LONG_PRESS;
        } else if (pressDuration >= SHORT_PRESS_DURATION_MS) {
            currentStatus = micras::proxy::Button::Status::LONG_PRESS;
        }
    } else if (!pressed) {
        // Button is not pressed
        currentStatus = micras::proxy::Button::Status::NO_PRESS;
    }
}

bool Box2DButton::isPressed() const {
    return pressed;
}

micras::proxy::Button::Status Box2DButton::getStatus() const {
    return currentStatus;
}

micrasverse::types::Vec2 Box2DButton::getPosition() const {
    b2Transform transform = b2Body_GetTransform(bodyId);
    b2Vec2 worldPos = b2TransformPoint(transform, localPosition);
    return {worldPos.x, worldPos.y};
}

}  // namespace micrasverse::physics 
