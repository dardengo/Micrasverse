#include "micras/proxy/proxy_bridge.hpp"
#include "physics/argb.hpp"
#include <iostream>
#include <algorithm>

namespace micras {

ProxyBridge::ProxyBridge(Micras& micras, micrasverse::physics::Box2DMicrasBody& micrasBody) 
    : micras(micras), micrasBody(micrasBody) {}

// Button access
proxy::Button::Status ProxyBridge::get_button_status() const {
    return micras.button_status;
}

bool ProxyBridge::is_button_pressed() const {
    return micras.button.is_pressed();
}

void ProxyBridge::set_button_status(proxy::Button::Status status) {
    micras.button.set_status(status);
}

// Buzzer access
void ProxyBridge::set_buzzer_frequency(float frequency) {
    micras.buzzer.set_frequency(frequency);
}

void ProxyBridge::set_buzzer_duration(uint32_t duration) {
    micras.buzzer.set_duration(duration);
}

void ProxyBridge::stop_buzzer() {
    micras.buzzer.stop();
}

bool ProxyBridge::is_buzzer_playing() const {
    return micras.buzzer.is_playing();
}

// Fan access
void ProxyBridge::set_fan_speed(float speed) {
    micras.fan.set_speed(speed);
}

float ProxyBridge::get_fan_speed() const {
    return micras.fan.get_speed();
}

void ProxyBridge::enable_fan() {
    micras.fan.enable();
}

void ProxyBridge::disable_fan() {
    micras.fan.disable();
}

void ProxyBridge::stop_fan() {
    micras.fan.stop();
}

// IMU access
void ProxyBridge::update_imu() {
    micras.imu->update();
}

float ProxyBridge::get_imu_acceleration_x() const {
    return micras.imu->get_linear_acceleration(proxy::Imu::Axis::X);
}

float ProxyBridge::get_imu_acceleration_y() const {
    return micras.imu->get_linear_acceleration(proxy::Imu::Axis::Y);
}

float ProxyBridge::get_imu_acceleration_z() const {
    return micras.imu->get_linear_acceleration(proxy::Imu::Axis::Z);
}

float ProxyBridge::get_imu_angular_velocity_x() const {
    return micras.imu->get_angular_velocity(proxy::Imu::Axis::X);
}

float ProxyBridge::get_imu_angular_velocity_y() const {
    return micras.imu->get_angular_velocity(proxy::Imu::Axis::Y);
}

float ProxyBridge::get_imu_angular_velocity_z() const {
    return micras.imu->get_angular_velocity(proxy::Imu::Axis::Z);
}

float ProxyBridge::get_imu_angular_velocity(proxy::Imu::Axis axis) const {
    return micras.imu->get_angular_velocity(axis);
}

float ProxyBridge::get_imu_linear_acceleration(proxy::Imu::Axis axis) const {
    return micras.imu->get_linear_acceleration(axis);
}

void ProxyBridge::calibrate_imu() {
    micras.imu->calibrate();
}

// Rotary sensors access
float ProxyBridge::get_left_rotary_sensor_position() const {
    return micras.rotary_sensor_left->get_position();
}

float ProxyBridge::get_right_rotary_sensor_position() const {
    return micras.rotary_sensor_right->get_position();
}

// Wall sensors access
void ProxyBridge::update_wall_sensors() {
    micras.wall_sensors->update();
}

float ProxyBridge::get_wall_sensor_distance(uint8_t index) const {
    return micras.wall_sensors->get_reading(index);
}

bool ProxyBridge::is_wall_detected(uint8_t index) const {
    return micras.wall_sensors->get_observation(index) == micras::core::Observation::WALL;
}

float ProxyBridge::get_wall_sensor_reading(uint8_t sensor_index) const {
    return micras.wall_sensors->get_reading(sensor_index);
}

float ProxyBridge::get_wall_sensor_adc_reading(uint8_t sensor_index) const {
    return micras.wall_sensors->get_adc_reading(sensor_index);
}

void ProxyBridge::calibrate_front_wall() {
    micras.wall_sensors->calibrate_front_wall();
}

void ProxyBridge::calibrate_left_wall() {
    micras.wall_sensors->calibrate_left_wall();
}

void ProxyBridge::calibrate_right_wall() {
    micras.wall_sensors->calibrate_right_wall();
}

void ProxyBridge::calibrate_front_free_space() {
    micras.wall_sensors->calibrate_front_free_space();
}

void ProxyBridge::calibrate_left_free_space() {
    micras.wall_sensors->calibrate_left_free_space();
}

void ProxyBridge::calibrate_right_free_space() {
    micras.wall_sensors->calibrate_right_free_space();
}

void ProxyBridge::turn_on_wall_sensors() {
    micras.wall_sensors->turn_on();
}

void ProxyBridge::turn_off_wall_sensors() {
    micras.wall_sensors->turn_off();
}

// Locomotion access
void ProxyBridge::set_wheel_command(float left_command, float right_command) {
    micras.locomotion.set_wheel_command(left_command, right_command);
}

void ProxyBridge::set_command(float linear, float angular) {
    micras.locomotion.set_command(linear, angular);
}

void ProxyBridge::stop_motors() {
    micras.locomotion.stop();
}

void ProxyBridge::enable_motors() {
    micras.locomotion.enable();
}

void ProxyBridge::disable_motors() {
    micras.locomotion.disable();
}

// ARGB access
void ProxyBridge::set_led_color(uint8_t index, uint8_t red, uint8_t green, uint8_t blue) {
    micrasverse::types::Color color{red, green, blue};
    micras.argb.set_color(color, index);
}

void ProxyBridge::set_led_brightness(uint8_t index, uint8_t brightness) {
    // The Micrasverse implementation doesn't have a direct set_brightness method
    // For now, we'll just use the set_color method with the current color
    // This is a workaround until a proper set_brightness method is implemented
    micrasverse::types::Color currentColor = micras.argb.argbRefs[index].get().getColor();
    float brightnessFactor = brightness / 255.0f;
    micrasverse::types::Color newColor{
        static_cast<uint8_t>(currentColor.r * brightnessFactor),
        static_cast<uint8_t>(currentColor.g * brightnessFactor),
        static_cast<uint8_t>(currentColor.b * brightnessFactor)
    };
    micras.argb.set_color(newColor, index);
}

void ProxyBridge::set_argb_color(const micrasverse::types::Color& color, uint8_t index) {
    micras.argb.set_color({color.r, color.g, color.b}, index);
}

void ProxyBridge::set_argb_color(const micrasverse::types::Color& color) {
    micras.argb.set_color({color.r, color.g, color.b});
}

void ProxyBridge::turn_off_argb(uint8_t index) {
    micras.argb.turn_off();
}

void ProxyBridge::turn_off_argb() {
    micras.argb.turn_off();
}

// Dip switch access
uint8_t ProxyBridge::get_dip_switch_value() const {
    return micras.dip_switch.get_switches_value();
}

bool ProxyBridge::get_dip_switch_state(uint8_t index) const {
    return micras.dip_switch.get_switch_state(index);
}

// Stopwatch access
void ProxyBridge::reset_stopwatch() {
    micras.loop_stopwatch.reset_us();
}

uint32_t ProxyBridge::get_elapsed_time_ms() const {
    return micras.loop_stopwatch.elapsed_time_ms();
}

uint32_t ProxyBridge::get_elapsed_time_us() const {
    return micras.loop_stopwatch.elapsed_time_us();
}

// MicrasController access
micras::core::Objective ProxyBridge::get_objective() const {
    // Return a default objective if not available
    return micras::core::Objective::EXPLORE;
}

micras::nav::Mapping::Action ProxyBridge::get_current_action() const {
    return micras.current_action;
}

std::string ProxyBridge::get_objective_string() const {
    switch (get_objective()) {
        case micras::core::Objective::EXPLORE:
            return "EXPLORE";
        case micras::core::Objective::RETURN:
            return "RETURN";
        case micras::core::Objective::SOLVE:
            return "SOLVE";
        default:
            return "UNKNOWN";
    }
}

std::string ProxyBridge::get_action_type_string() const {
    switch (get_current_action().type) {
        case micras::nav::Mapping::Action::Type::LOOK_AT:
            return "LOOK_AT";
        case micras::nav::Mapping::Action::Type::GO_TO:
            return "GO_TO";
        case micras::nav::Mapping::Action::Type::ALIGN_BACK:
            return "ALIGN_BACK";
        case micras::nav::Mapping::Action::Type::FINISHED:
            return "FINISHED";
        case micras::nav::Mapping::Action::Type::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

micras::nav::Point ProxyBridge::get_current_goal() const {
    // Return the point from the current action instead of a placeholder
    return micras.current_action.point;
}

micras::nav::Pose ProxyBridge::get_current_pose() const {
    micras::nav::State state = micras.odometry.get_state();

    return state.pose;
}

} // namespace micras::proxy 
