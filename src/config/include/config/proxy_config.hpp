#pragma once

#include "proxy/battery.hpp"
#include "proxy/button.hpp"
#include "proxy/buzzer.hpp"
#include "proxy/dip_switch.hpp"
#include "proxy/fan.hpp"
#include "proxy/led.hpp"
#include "proxy/stopwatch.hpp"
#include "proxy/storage.hpp"
#include "proxy/torque_sensors.hpp"

namespace micras::proxy {

// Battery configuration
const Battery::Config battery_config = {
    .voltage = 12.0f,              // V
    .voltage_divider = 11.0f,      // R1 = 10k, R2 = 1k
    .filter_cutoff = 10.0f,        // Hz
    .noise = 0.1f                  // V
};

// Button configuration
const Button::Config button_config = {
    .pull_resistor = Button::PullResistor::PULL_UP,
    .debounce_delay = 50,          // ms
    .long_press_delay = 1000,      // ms
    .extra_long_press_delay = 3000 // ms
};

// Buzzer configuration
const Buzzer::Config buzzer_config = {
    .volume = 0.5f                 // 0.0 to 1.0
};

// DIP switch configuration
const DipSwitch::Config dip_switch_config = {
    .num_switches = 4,             // Number of DIP switches
    .initial_state = 0b0000        // Initial state (all off)
};

// Fan configuration
const Fan::Config fan_config = {
    .max_speed = 1.0f,             // 0.0 to 1.0
    .min_speed = 0.0f,             // 0.0 to 1.0
    .startup_speed = 0.5f          // 0.0 to 1.0
};

// LED configuration
const Led::Config led_config = {
    .brightness = 1.0f,            // 0.0 to 1.0
    .initial_state = false         // Initially off
};

// Stopwatch configuration
const Stopwatch::Config stopwatch_config = {
    .resolution = 1,               // ms
    .max_time = 3600000           // 1 hour in ms
};

// Storage configuration
const Storage::Config storage_config = {
    .page_size = 256,              // bytes
    .num_pages = 32,               // Total storage size = 8KB
    .write_delay = 5               // ms
};

// Torque sensors configuration
const TorqueSensors::Config torque_sensors_config = {
    .num_sensors = 2,              // Left and right motor
    .sensitivity = 0.1f,           // Nm/V
    .filter_cutoff = 100.0f,       // Hz
    .noise = 0.01f                 // Nm
};

} // namespace micras::proxy 
