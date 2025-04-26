#include "micras/proxy/button.hpp"

namespace micras::proxy {

Button::Button(const Config& config) :
    micrasBody(config.micrasBody),
    current_state(config.initial_state),
    previous_state(config.initial_state),
    current_status(Status::NO_PRESS),
    pull_type(config.pull_type) {
}

bool Button::is_pressed() const {
    // Return the logical state after applying pull resistor effect
    return get_logical_state();
}

void Button::update() {
}

Button::Status Button::get_status() const {
    return current_status;
}

void Button::set_state(bool state) {
    // Store previous state for edge detection
    previous_state = current_state;
    current_state = state;
    
    // Update status based on logical state (after pull effect)
    bool logical_state = get_logical_state();
    
    if (!logical_state) {
        // Button is released
        current_status = Status::NO_PRESS;
    } else if (current_status == Status::NO_PRESS) {
        // Button is pressed and was previously released
        current_status = Status::SHORT_PRESS;
    }
}

void Button::set_status(Status status) {
    current_status = status;
    
    // Update raw state based on status and pull type
    if (status == Status::NO_PRESS) {
        // If no press, set raw state based on pull type
        // For PULL_UP: button not pressed = input pulled high = true
        // For PULL_DOWN: button not pressed = input pulled low = false
        current_state = (pull_type == PullType::PULL_UP);
    } else {
        // If pressed, set raw state based on pull type (inverted)
        // For PULL_UP: button pressed = input pulled low = false
        // For PULL_DOWN: button pressed = input pulled high = true
        current_state = (pull_type == PullType::PULL_DOWN);
    }
}

Button::PullType Button::get_pull_type() const {
    return pull_type;
}

void Button::set_pull_type(PullType pull_type) {
    this->pull_type = pull_type;
}

bool Button::get_logical_state() const {
    // Apply pull resistor effect to get logical state
    // For PULL_UP: true = not pressed, false = pressed
    // For PULL_DOWN: true = pressed, false = not pressed
    return (pull_type == PullType::PULL_UP) ? !current_state : current_state;
}

} // namespace micras::proxy 
