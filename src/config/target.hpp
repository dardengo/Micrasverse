#ifndef MICRAS_TARGET_HPP
#define MICRAS_TARGET_HPP

#include "micras/proxy/argb.hpp"
#include "micras/proxy/battery.hpp"
#include "micras/proxy/button.hpp"
#include "micras/proxy/buzzer.hpp"
#include "micras/proxy/dip_switch.hpp"
#include "micras/proxy/fan.hpp"
#include "micras/proxy/imu.hpp"
#include "micras/proxy/led.hpp"
#include "micras/proxy/locomotion.hpp"
#include "micras/proxy/rotary_sensor.hpp"
#include "micras/proxy/stopwatch.hpp"
#include "micras/proxy/storage.hpp"
#include "micras/proxy/torque_sensors.hpp"
#include "micras/proxy/wall_sensors.hpp"
#include "micras/proxy/motor.hpp"
#include "box2d/box2d.h"
#include <filesystem>

namespace micras {

/*****************************************
 * Template Instantiations
 *****************************************/

namespace proxy {
using Argb = proxy::TArgb<2>;
using DipSwitch = TDipSwitch<4>;
using TorqueSensors = TTorqueSensors<2>;
using WallSensors = TWallSensors<4>;
}  // namespace proxy

/*****************************************
 * Proxy Configurations
 *****************************************/
namespace proxy {

// ARGB LED strip configuration
static constexpr Argb::Config ArgbConfig = {
    .bodyId = b2BodyId{},
    .uncertainty = 0.0f,
    .brightness = {1.0f, 1.0f}
};

// Battery configuration
static constexpr Battery::Config BatteryConfig = {
    .bodyId = b2BodyId{},
    .voltage = 12.0f,
    .voltage_divider = 1.0f,
    .filter_cutoff = 1.0f,
    .noise = 0.0f
};

// Button configuration
static constexpr Button::Config ButtonConfig = {
    .bodyId = b2BodyId{0, 0},  // This will be set by Box2DMicrasBody
    .pull_resistor = Button::PullResistor::PULL_UP,
    .debounce_delay = 50,  // ms
    .long_press_delay = 1000,  // ms
    .extra_long_press_delay = 3000,  // ms
    .initial_state = false
};

// Buzzer configuration
static constexpr Buzzer::Config BuzzerConfig = {
    .bodyId = b2BodyId{},
    .volume = 255
};

// DIP Switch configuration
static constexpr DipSwitch::Config DipSwitchConfig = {
    .bodyId = b2BodyId{},
    .initial_states = {false, false, false, false}
};

// Fan configuration
static constexpr Fan::Config FanConfig = {
    .bodyId = b2BodyId{},
    .max_acceleration = 100.0f,  // Maximum acceleration in speed units per second
    .max_speed = 100.0f         // Maximum speed in speed units
};

// IMU configuration
static constexpr Imu::Config ImuConfig = {
    .bodyId = b2BodyId{},
    .gyroscope_noise = 0.0f,
    .accelerometer_noise = 0.0f
};

// LED configuration
static constexpr Led::Config LedConfig = {
    .bodyId = b2BodyId{},
    .initial_state = false,
    .red = 255,
    .green = 255,
    .blue = 255
};

// Motor configuration
static constexpr Motor::Config MotorConfig = {
    .bodyId = b2BodyId{},
    .max_speed = 100.0f,  // Maximum speed in RPM
    .max_torque = 1.0f,   // Maximum torque in Nm
    .gear_ratio = 1.0f    // Gear ratio
};

// Rotary Sensor configuration
static constexpr RotarySensor::Config RotarySensorConfig = {
    .bodyId = b2BodyId{},
    .resolution = 1000.0f,  // Resolution of the encoder in counts per revolution
    .noise = 0.0f          // Noise level in counts
};

// Stopwatch configuration
static constexpr Stopwatch::Config StopwatchConfig = {
    .bodyId = b2BodyId{}
};

// Storage configuration
static Storage::Config StorageConfig{
    .storage_path = std::filesystem::path{},
    .bodyId = b2BodyId{}
};

// Torque Sensors configuration
static constexpr TorqueSensors::Config TorqueSensorsConfig = {
    .bodyId = b2BodyId{},
    .shunt_resistor = 0.1f,
    .max_torque = 1.0f,
    .filter_cutoff = 1.0f,
    .noise = 0.0f
};

// Wall Sensors configuration
static constexpr WallSensors::Config WallSensorsConfig = {
    .bodyId = b2BodyId{},
    .uncertainty = 0.0f,
    .wall_threshold = {3.0f, 3.0f, 3.0f, 3.0f},
    .free_threshold = {5.0f, 5.0f, 5.0f, 5.0f}
};

// Locomotion configuration
static constexpr Locomotion::Config LocomotionConfig = {
    .bodyId = b2BodyId{}
};

}  // namespace proxy

}  // namespace micras

#endif  //  MICRAS_TARGET_HPP
