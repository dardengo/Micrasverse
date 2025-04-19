#include "micras/proxy/gui_controller.hpp"
#include <imgui.h>

namespace micras::proxy {

GuiController::GuiController(const Config& config, std::shared_ptr<ProxyBridge> proxy_bridge)
    : config(config), proxy_bridge(proxy_bridge) {}

void GuiController::update() {
    // Update GUI element states from proxy bridge
    button_pressed = proxy_bridge->is_button_pressed();
    dip_switch_state = proxy_bridge->get_dip_switch_value();
    
    // Only try to set LED colors if they exist
    if (proxy_bridge && proxy_bridge->get_argb_count() > 0) {
        micrasverse::types::Color color = {0, 0, 0};
        proxy_bridge->set_argb_color(color, 0);
        led_state = false; // If any LED was on, it's now off
    }
    
    fan_speed = proxy_bridge->get_fan_speed();
    buzzer_playing = proxy_bridge->is_buzzer_playing();
}

void GuiController::render() {
    // Create a main window that contains all controls
    ImGui::SetNextWindowPos(ImVec2(config.button.x, config.button.y));
    ImGui::SetNextWindowSize(ImVec2(config.button.width + 20, config.buzzer.y + config.buzzer.height + 20));
    ImGui::Begin("Micras Controls", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    // Add a bit of padding
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));

    // Button section with enhanced status visualization
    ImGui::BeginChild("Button Section", ImVec2(0, 80), true);
    
    // Get current button status
    auto status = proxy_bridge->get_button_status();
    std::string statusText;
    ImVec4 statusColor;
    
    switch (status) {
        case micras::proxy::Button::Status::NO_PRESS:
            statusText = "RELEASED";
            statusColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
            break;
        case micras::proxy::Button::Status::SHORT_PRESS:
            statusText = "SHORT PRESS";
            statusColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            break;
        case micras::proxy::Button::Status::LONG_PRESS:
            statusText = "LONG PRESS";
            statusColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
            break;
        case micras::proxy::Button::Status::EXTRA_LONG_PRESS:
            statusText = "EXTRA LONG PRESS";
            statusColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            break;
        default:
            statusText = "UNKNOWN";
            statusColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
            break;
    }
    
    // Display button status with enhanced styling
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
    ImGui::PushStyleColor(ImGuiCol_Button, statusColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(statusColor.x * 1.2f, statusColor.y * 1.2f, statusColor.z * 1.2f, statusColor.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(statusColor.x * 0.8f, statusColor.y * 0.8f, statusColor.z * 0.8f, statusColor.w));
    
    // Create a button that shows the status (non-interactive)
    ImGui::Button(statusText.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 40));
    
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();
    
    // Add debug information
    ImGui::Text("Raw State: %s", button_pressed ? "PRESSED" : "RELEASED");
    ImGui::Text("Pull Type: %s", proxy_bridge->get_button_pull_type() == micras::proxy::Button::PullType::PULL_UP ? "PULL UP" : "PULL DOWN");
    
    ImGui::EndChild();

    ImGui::Spacing();

    // DIP Switch section
    ImGui::BeginChild("DIP Switch Section", ImVec2(0, 90), true);
    ImGui::Text("DIP Switch Value:");
    ImGui::SameLine();
    for (int i = 3; i >= 0; i--) {
        ImGui::SameLine();
        bool bit_set = (dip_switch_state & (1 << i)) != 0;
        ImGui::PushStyleColor(ImGuiCol_Text, bit_set ? ImVec4(0, 1, 0, 1) : ImVec4(0.5f, 0.5f, 0.5f, 1));
        ImGui::Text("%d", bit_set ? 1 : 0);
        ImGui::PopStyleColor();
    }
    ImGui::NewLine();
    ImGui::Text("Bit:  3     2     1     0");
    ImGui::Text("     TURBO  STOP   FAN   DIAG");
    ImGui::EndChild();

    ImGui::Spacing();

    // LED section
    ImGui::BeginChild("LED Section", ImVec2(0, 50), true);
    ImGui::Text("LED Status:");
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, led_state ? ImVec4(1, 1, 1, 1) : ImVec4(0.5f, 0.5f, 0.5f, 1));
    if (ImGui::Button(led_state ? "ON" : "OFF", ImVec2(50, 0))) {
        led_state = !led_state;
        if (proxy_bridge && proxy_bridge->get_argb_count() > 0) {
            if (led_state) {
                proxy_bridge->set_led_color(0, 255, 255, 255);
            } else {
                proxy_bridge->set_led_color(0, 0, 0, 0);
            }
        }
    }
    ImGui::PopStyleColor();
    ImGui::EndChild();

    ImGui::Spacing();

    // Battery section
    ImGui::BeginChild("Battery Section", ImVec2(0, 50), true);
    ImGui::Text("Battery Status:");
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
    ImGui::Text("OK");
    ImGui::PopStyleColor();
    ImGui::EndChild();

    ImGui::Spacing();

    // Fan section
    ImGui::BeginChild("Fan Section", ImVec2(0, 70), true);
    ImGui::Text("Fan Control:");
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.3f, 0.7f, 0.9f, 1));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.4f, 0.8f, 1.0f, 1));
    if (ImGui::SliderFloat("##fan", &fan_speed, 0.0f, 100.0f, "%.0f%%")) {
        proxy_bridge->set_fan_speed(fan_speed);
    }
    ImGui::PopStyleColor(2);
    ImGui::EndChild();

    ImGui::Spacing();

    // Buzzer section
    ImGui::BeginChild("Buzzer Section", ImVec2(0, 50), true);
    ImGui::Text("Buzzer:");
    ImGui::SameLine();
    if (ImGui::Button(buzzer_playing ? "Stop" : "Start", ImVec2(60, 0))) {
        if (!buzzer_playing) {
            proxy_bridge->set_buzzer_frequency(1000);
            proxy_bridge->set_buzzer_duration(100);
        } else {
            proxy_bridge->stop_buzzer();
        }
    }
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, buzzer_playing ? ImVec4(1, 1, 0, 1) : ImVec4(0.5f, 0.5f, 0.5f, 1));
    ImGui::Text("%s", buzzer_playing ? "ACTIVE" : "IDLE");
    ImGui::PopStyleColor();
    ImGui::EndChild();

    // Pop style variables
    ImGui::PopStyleVar(2);

    ImGui::End();
}

void GuiController::handle_mouse_click(float x, float y) {
    update_button_state(x, y);
    update_dip_switch_state(x, y);
    update_led_state(x, y);
    update_fan_speed(x, y);
    update_buzzer_state(x, y);
}

void GuiController::handle_mouse_release(float x, float y) {
    // Handle any cleanup needed on mouse release
}

void GuiController::handle_mouse_drag(float x, float y) {
    update_fan_speed(x, y);
}

bool GuiController::is_point_in_rect(float x, float y, float rect_x, float rect_y, float rect_width, float rect_height) {
    return x >= rect_x && x <= rect_x + rect_width && y >= rect_y && y <= rect_y + rect_height;
}

void GuiController::update_button_state(float x, float y) {
    if (is_point_in_rect(x, y, config.button.x, config.button.y, config.button.width, config.button.height)) {
        button_pressed = !button_pressed;
    }
}

void GuiController::update_dip_switch_state(float x, float y) {
    if (is_point_in_rect(x, y, config.dip_switch.x, config.dip_switch.y, config.dip_switch.width, config.dip_switch.height)) {
        // Toggle dip switch state
        uint8_t new_state = (dip_switch_state + 1) % 16; // 4-bit dip switch
        // Since we can't directly set dip switch state, we'll just update our local state
        dip_switch_state = new_state;
    }
}

void GuiController::update_led_state(float x, float y) {
    if (is_point_in_rect(x, y, config.led.x, config.led.y, config.led.width, config.led.height)) {
        // Toggle LED state using ARGB
        if (proxy_bridge && proxy_bridge->get_argb_count() > 0) {
            if (!led_state) {
                proxy_bridge->set_led_color(0, 255, 255, 255); // White when on
            } else {
                proxy_bridge->set_led_color(0, 0, 0, 0); // Off
            }
        }
    }
}

void GuiController::update_fan_speed(float x, float y) {
    if (is_point_in_rect(x, y, config.fan.x, config.fan.y, config.fan.width, config.fan.height)) {
        float normalized_x = (x - config.fan.x) / config.fan.width;
        float new_speed = normalized_x * 100.0f; // Scale to 0-100%
        proxy_bridge->set_fan_speed(new_speed);
    }
}

void GuiController::update_buzzer_state(float x, float y) {
    if (is_point_in_rect(x, y, config.buzzer.x, config.buzzer.y, config.buzzer.width, config.buzzer.height)) {
        if (!buzzer_playing) {
            proxy_bridge->set_buzzer_frequency(1000); // Default frequency
            proxy_bridge->set_buzzer_duration(100);   // Default duration (ms)
        } else {
            proxy_bridge->stop_buzzer();
        }
    }
}

} // namespace micras::proxy 
