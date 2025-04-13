#include "micras/proxy/gui_controller.hpp"

namespace micras::proxy {

GuiController::GuiController(const Config& config,
                           std::shared_ptr<Button> button,
                           std::shared_ptr<TDipSwitch<4>> dip_switch,
                           std::shared_ptr<Led> led,
                           std::shared_ptr<Battery> battery,
                           std::shared_ptr<Fan> fan,
                           std::shared_ptr<Buzzer> buzzer)
    : config(config)
    , button(button)
    , dip_switch(dip_switch)
    , led(led)
    , battery(battery)
    , fan(fan)
    , buzzer(buzzer) {}

void GuiController::update() {
    // Update component states
    button_pressed = button->is_pressed();
    dip_switch_state = dip_switch->get_switches_value();
    led_state = led->is_on();
    fan_speed = fan->get_speed();
    buzzer_playing = buzzer->is_playing();
}

void GuiController::render() {
    render_button();
    render_dip_switch();
    render_led();
    render_battery();
    render_fan();
    render_buzzer();
}

void GuiController::handle_mouse_click(float x, float y) {
    update_button_state(x, y);
    update_dip_switch_state(x, y);
    update_led_state(x, y);
    update_fan_speed(x, y);
    update_buzzer_state(x, y);
}

void GuiController::handle_mouse_release(float x, float y) {
    // Handle any release events if needed
}

void GuiController::handle_mouse_drag(float x, float y) {
    update_fan_speed(x, y);
}

bool GuiController::is_point_in_rect(float x, float y, float rect_x, float rect_y, float rect_width, float rect_height) {
    return x >= rect_x && x <= rect_x + rect_width && y >= rect_y && y <= rect_y + rect_height;
}

void GuiController::update_button_state(float x, float y) {
    if (is_point_in_rect(x, y, config.button.x, config.button.y, config.button.width, config.button.height)) {
        // Button doesn't have set_pressed method, we can only check its state
        button_pressed = true;
    }
}

void GuiController::update_dip_switch_state(float x, float y) {
    if (is_point_in_rect(x, y, config.dip_switch.x, config.dip_switch.y, config.dip_switch.width, config.dip_switch.height)) {
        // Toggle the dip switch state
        uint8_t new_state = (dip_switch_state + 1) % 16;
        dip_switch->setState(new_state);
    }
}

void GuiController::update_led_state(float x, float y) {
    if (is_point_in_rect(x, y, config.led.x, config.led.y, config.led.width, config.led.height)) {
        led->toggle();
    }
}

void GuiController::update_fan_speed(float x, float y) {
    if (is_point_in_rect(x, y, config.fan.x, config.fan.y, config.fan.width, config.fan.height)) {
        float normalized_x = (x - config.fan.x) / config.fan.width;
        float speed = normalized_x * 100.0f;  // Scale to 0-100%
        fan->set_speed(speed);
    }
}

void GuiController::update_buzzer_state(float x, float y) {
    if (is_point_in_rect(x, y, config.buzzer.x, config.buzzer.y, config.buzzer.width, config.buzzer.height)) {
        if (!buzzer_playing) {
            buzzer->play(440, 100); // A4 note, 100ms duration
        } else {
            buzzer->stop();
        }
    }
}

void GuiController::render_button() {
    // Render button state
    // Implementation depends on your rendering system
}

void GuiController::render_dip_switch() {
    // Render dip switch state
    // Implementation depends on your rendering system
}

void GuiController::render_led() {
    // Render LED state
    // Implementation depends on your rendering system
}

void GuiController::render_battery() {
    // Render battery level
    // Implementation depends on your rendering system
}

void GuiController::render_fan() {
    // Render fan speed
    // Implementation depends on your rendering system
}

void GuiController::render_buzzer() {
    // Render buzzer state
    // Implementation depends on your rendering system
}

} // namespace micras::proxy 
