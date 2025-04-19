#include "target.hpp"
#include <filesystem>

namespace micras {

// ARGB LED strip configuration
proxy::Argb::Config argb_config = {
    .bodyId = b2BodyId{},
    .uncertainty = 0.0f,
    .brightness = {1.0f, 1.0f}
};

// Battery configuration
proxy::Battery::Config battery_config = {
    .bodyId = b2BodyId{},
    .voltage = 12.0f,
    .voltage_divider = 1.0f,
    .filter_cutoff = 1.0f,
    .noise = 0.0f
};

// Button configuration
proxy::Button::Config button_config = {
    .bodyId = b2BodyId{0, 0},  // This will be set by Box2DMicrasBody
    .worldId = b2WorldId{0, 0},  // This will be set by Box2DMicrasBody
    .initial_state = false,
    .pull_type = proxy::Button::PullType::PULL_UP
};

// Buzzer configuration
proxy::Buzzer::Config buzzer_config = {
    .bodyId = b2BodyId{},
    .volume = 255
};

// DIP Switch configuration
proxy::DipSwitch::Config dip_switch_config = {
    .bodyId = b2BodyId{},
    .initial_states = {false, false, false, false}
};

// Fan configuration
proxy::Fan::Config fan_config = {
    .bodyId = b2BodyId{},
    .worldId = b2WorldId{},
    .max_acceleration = 100.0f,  // Maximum acceleration in speed units per second
    .max_speed = 100.0f         // Maximum speed in speed units
};

// IMU configuration
proxy::Imu::Config imu_config = {
    .bodyId = b2BodyId{},
    .gyroscope_noise = 0.0f,
    .accelerometer_noise = 0.0f
};

// LED configuration
proxy::Led::Config led_config = {
    .bodyId = b2BodyId{},
    .initial_state = false,
    .red = 255,
    .green = 255,
    .blue = 255
};

// Motor configuration
proxy::Motor::Config motor_config = {
    .bodyId = b2BodyId{},
    .worldId = b2WorldId{},
    .max_speed = 100.0f,  // Maximum speed in RPM
    .max_torque = 1.0f,   // Maximum torque in Nm
    .gear_ratio = 1.0f    // Gear ratio
};

// Rotary Sensor configuration
proxy::RotarySensor::Config rotary_sensor_left_config = {
    .bodyId = b2BodyId{},
    .resolution = 1000.0f,  // Resolution of the encoder in counts per revolution
    .noise = 0.0f          // Noise level in counts
};

proxy::RotarySensor::Config rotary_sensor_right_config = {
    .bodyId = b2BodyId{},
    .resolution = 1000.0f,  // Resolution of the encoder in counts per revolution
    .noise = 0.0f          // Noise level in counts
};

// Stopwatch configuration
proxy::Stopwatch::Config stopwatch_config = {
    
};

// Storage configuration
proxy::Storage::Config maze_storage_config{
    .storage_path = std::filesystem::path{"storage/maze"},
    .bodyId = b2BodyId{}
};

// Torque Sensors configuration
proxy::TorqueSensors::Config torque_sensors_config = {
    .bodyId = b2BodyId{},
    .shunt_resistor = 0.1f,
    .max_torque = 1.0f,
    .filter_cutoff = 1.0f,
    .noise = 0.0f
};

// Wall Sensors configuration
proxy::WallSensors::Config wall_sensors_config = {
    .bodyId = b2BodyId{},
    .uncertainty = 0.0f,
    .wall_threshold = {3.0f, 3.0f, 3.0f, 3.0f},
    .free_threshold = {5.0f, 5.0f, 5.0f, 5.0f}
};

// Locomotion configuration
proxy::Locomotion::Config locomotion_config = {
    .bodyId = b2BodyId{},
    .left_motor = nullptr,
    .right_motor = nullptr
};

// Function to initialize all proxy configs with the correct bodyId
void initializeProxyConfigs(b2BodyId bodyId, b2WorldId worldId) {
    argb_config.bodyId = bodyId;
    battery_config.bodyId = bodyId;
    button_config.bodyId = bodyId;
    button_config.worldId = worldId;
    buzzer_config.bodyId = bodyId;
    dip_switch_config.bodyId = bodyId;
    fan_config.bodyId = bodyId;
    fan_config.worldId = worldId;
    imu_config.bodyId = bodyId;
    led_config.bodyId = bodyId;
    locomotion_config.bodyId = bodyId;
    motor_config.bodyId = bodyId;
    motor_config.worldId = worldId;
    rotary_sensor_left_config.bodyId = bodyId;
    rotary_sensor_right_config.bodyId = bodyId;
    maze_storage_config.bodyId = bodyId;
    torque_sensors_config.bodyId = bodyId;
    wall_sensors_config.bodyId = bodyId;
}

}  // namespace micras 
