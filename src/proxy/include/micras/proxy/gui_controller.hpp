#pragma once

#include <memory>
#include <vector>
#include "micras/proxy/proxy_bridge.hpp"

namespace micras::proxy {

class GuiController {
public:
    struct Config {
        // GUI element positions and sizes (in screen coordinates)
        struct {
            float x = 10.0f;
            float y = 10.0f;
            float width = 200.0f;
            float height = 30.0f;
        } button;
        
        struct {
            float x = 10.0f;
            float y = 50.0f;
            float width = 200.0f;
            float height = 30.0f;
        } dip_switch;
        
        struct {
            float x = 10.0f;
            float y = 90.0f;
            float width = 200.0f;
            float height = 30.0f;
        } led;
        
        struct {
            float x = 10.0f;
            float y = 130.0f;
            float width = 200.0f;
            float height = 30.0f;
        } battery;
        
        struct {
            float x = 10.0f;
            float y = 170.0f;
            float width = 200.0f;
            float height = 30.0f;
        } fan;
        
        struct {
            float x = 10.0f;
            float y = 210.0f;
            float width = 200.0f;
            float height = 30.0f;
        } buzzer;
    };

    GuiController(const Config& config, std::shared_ptr<ProxyBridge> proxy_bridge);

    void update();
    void render();
    void handle_mouse_click(float x, float y);
    void handle_mouse_release(float x, float y);
    void handle_mouse_drag(float x, float y);

private:
    Config config;
    std::shared_ptr<ProxyBridge> proxy_bridge;
    
    // GUI element states
    bool button_pressed = false;
    uint8_t dip_switch_state = 0;
    bool led_state = false;
    float fan_speed = 0.0f;
    bool buzzer_playing = false;
    
    // Helper methods for GUI element interaction
    bool is_point_in_rect(float x, float y, float rect_x, float rect_y, float rect_width, float rect_height);
    void update_button_state(float x, float y);
    void update_dip_switch_state(float x, float y);
    void update_led_state(float x, float y);
    void update_fan_speed(float x, float y);
    void update_buzzer_state(float x, float y);
    
    // Rendering helpers
    void render_button();
    void render_dip_switch();
    void render_led();
    void render_battery();
    void render_fan();
    void render_buzzer();
};

} // namespace micras::proxy 
