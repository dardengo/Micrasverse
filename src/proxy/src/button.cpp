#include "micras/proxy/button.hpp"

namespace micras::proxy {

Button::Button(const Config& config) :
    bodyId(config.bodyId),
    pull_resistor(config.pull_resistor),
    debounce_delay(config.debounce_delay),
    long_press_delay(config.long_press_delay),
    extra_long_press_delay(config.extra_long_press_delay),
    simulated_state(config.initial_state),
    last_state(false),
    current_state(false),
    debouncing(false),
    debounce_timer(std::make_unique<Stopwatch>(Stopwatch::Config{config.bodyId})),
    press_timer(std::make_unique<Stopwatch>(Stopwatch::Config{config.bodyId})) {
}

bool Button::is_pressed() {
    update_state();
    return current_state;
}

Button::Status Button::get_status() {
    update_state();
    
    if (!current_state) {
        return Status::NO_PRESS;
    }

    uint32_t press_duration = press_timer->elapsed_time_ms();
    
    if (press_duration >= extra_long_press_delay) {
        return Status::EXTRA_LONG_PRESS;
    } else if (press_duration >= long_press_delay) {
        return Status::LONG_PRESS;
    } else {
        return Status::SHORT_PRESS;
    }
}

void Button::set_state(bool state) {
    simulated_state = state;
}

bool Button::get_raw_reading() const {
    return simulated_state;
}

void Button::update_state() {
    bool raw_state = get_raw_reading();
    
    if (raw_state != last_state) {
        if (!debouncing) {
            debouncing = true;
            debounce_timer->reset_ms();
        } else if (debounce_timer->elapsed_time_ms() >= debounce_delay) {
            current_state = raw_state;
            if (current_state) {
                press_timer->reset_ms();
            }
            debouncing = false;
        }
    } else if (debouncing && debounce_timer->elapsed_time_ms() >= debounce_delay) {
        current_state = raw_state;
        if (current_state) {
            press_timer->reset_ms();
        }
        debouncing = false;
    }
    
    last_state = raw_state;
}

bool Button::is_rising_edge() const {
    return current_state && !last_state;
}

bool Button::is_falling_edge() const {
    return !current_state && last_state;
}

} // namespace micras::proxy 
