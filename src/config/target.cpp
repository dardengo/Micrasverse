#include "target.hpp"
#include <filesystem>

namespace micras {

// ARGB LED strip configuration
proxy::Argb::Config argb_config = {
    .micrasBody = nullptr,
    .uncertainty = 0.0f,
    .brightness = {1.0f, 1.0f}
};

// Battery configuration
proxy::Battery::Config battery_config = {
    .micrasBody = nullptr,
    .voltage = 12.0f,
    .voltage_divider = 1.0f,
    .filter_cutoff = 1.0f,
    .noise = 0.0f
};

// Button configuration
proxy::Button::Config button_config = {
    .micrasBody = nullptr,
    .initial_state = false,
    .pull_type = proxy::Button::PullType::PULL_UP
};

// Buzzer configuration
proxy::Buzzer::Config buzzer_config = {
    .micrasBody = nullptr,
    .volume = 255
};

// DIP Switch configuration
proxy::DipSwitch::Config dip_switch_config = {
    .micrasBody = nullptr,
    .initial_states = {false, false, false, false}
};

// Fan configuration
proxy::Fan::Config fan_config = {
    .micrasBody = nullptr,
    .max_acceleration = 100.0f,  // Maximum acceleration in speed units per second
    .max_speed = 100.0f          // Maximum speed in speed units
};

// IMU configuration
proxy::Imu::Config imu_config = {
    .micrasBody = nullptr,
    .gyroscope_noise = 0.0f,
    .accelerometer_noise = 0.0f
};

// LED configuration
proxy::Led::Config led_config = {
    .micrasBody = nullptr,
    .initial_state = false,
    .red = 255,
    .green = 255,
    .blue = 255
};

// Motor configuration
proxy::Motor::Config motor_config = {
    .micrasBody = nullptr,
    .isLeftWheel = true,  // Will be set per motor
    .max_speed = 100.0f,  // Maximum speed in RPM
    .max_torque = 0.0008f,   // Maximum torque in Nm
    .gear_ratio = 2.5f    // Gear ratio
};

// Rotary Sensor configuration
proxy::RotarySensor::Config rotary_sensor_left_config = {
    .micrasBody = nullptr,
    .resolution = 4096.0f,  // Resolution of the encoder in counts per revolution
    .noise = 0.0f,           // Noise level in counts
    .isLeftWheel = true
};

proxy::RotarySensor::Config rotary_sensor_right_config = {
    .micrasBody = nullptr,
    .resolution = 4096.0f,  // Resolution of the encoder in counts per revolution
    .noise = 0.0f,           // Noise level in counts
    .isLeftWheel = false
};

// Stopwatch configuration
proxy::Stopwatch::Config stopwatch_config = {
};

// Storage configuration
proxy::Storage::Config maze_storage_config{
    .storage_path = std::filesystem::path{"storage/maze"}
};

// Torque Sensors configuration
proxy::TorqueSensors::Config torque_sensors_config = {
    .micrasBody = nullptr,
    .shunt_resistor = 0.1f,
    .max_torque = 1.0f,
    .filter_cutoff = 1.0f,
    .noise = 0.0f
};

// Wall Sensors configuration
proxy::WallSensors::Config wall_sensors_config = {
    .micrasBody = nullptr,
    .uncertainty = 0.8f,
    .base_readings = {0.5861f, 0.7594f, 0.7594f, 0.5861f},
    .K = 1.0f,
    .max_adc_reading = 1.0f,
    .max_distance = 0.18f*2.0f
};

// Locomotion configuration
proxy::Locomotion::Config locomotion_config = {
    .micrasBody = nullptr
};

void initializeProxyConfigs(micrasverse::physics::Box2DMicrasBody* body) {
    argb_config.micrasBody = body;
    battery_config.micrasBody = body;
    button_config.micrasBody = body;
    buzzer_config.micrasBody = body;
    dip_switch_config.micrasBody = body;
    fan_config.micrasBody = body;
    imu_config.micrasBody = body;
    led_config.micrasBody = body;
    motor_config.micrasBody = body;
    rotary_sensor_left_config.micrasBody = body;
    rotary_sensor_right_config.micrasBody = body;
    torque_sensors_config.micrasBody = body;
    wall_sensors_config.micrasBody = body;
    locomotion_config.micrasBody = body;
}

}  // namespace micras 
