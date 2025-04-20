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
#include "physics/box2d_micrasbody.hpp"

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

// ARGB LED strip configuration
extern proxy::Argb::Config argb_config;

// Battery configuration
extern proxy::Battery::Config battery_config;

// Button configuration
extern proxy::Button::Config button_config;

// Buzzer configuration
extern proxy::Buzzer::Config buzzer_config;

// DIP Switch configuration
extern proxy::DipSwitch::Config dip_switch_config;

// Fan configuration
extern proxy::Fan::Config fan_config;

// IMU configuration
extern proxy::Imu::Config imu_config;

// LED configuration
extern proxy::Led::Config led_config;

// Motor configuration
extern proxy::Motor::Config motor_config;

// Rotary Sensor configuration
extern proxy::RotarySensor::Config rotary_sensor_left_config;
extern proxy::RotarySensor::Config rotary_sensor_right_config;

// Stopwatch configuration
extern proxy::Stopwatch::Config stopwatch_config;

// Storage configuration
extern proxy::Storage::Config maze_storage_config;

// Torque Sensors configuration
extern proxy::TorqueSensors::Config torque_sensors_config;

// Wall Sensors configuration
extern proxy::WallSensors::Config wall_sensors_config;

// Locomotion configuration
extern proxy::Locomotion::Config locomotion_config;

// Function to initialize all proxy configs with the correct bodyId
void initializeProxyConfigs(micrasverse::physics::Box2DMicrasBody* body);

}  // namespace micras

#endif  //  MICRAS_TARGET_HPP
