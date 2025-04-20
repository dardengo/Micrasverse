#ifndef MICRAS_PROXY_TORQUE_SENSORS_CPP
#define MICRAS_PROXY_TORQUE_SENSORS_CPP

#include "micras/proxy/torque_sensors.hpp"
#include <algorithm>

namespace micras::proxy {

template <uint8_t num_of_sensors>
TTorqueSensors<num_of_sensors>::TTorqueSensors(const Config& config) :
    micrasBody{config.micrasBody},
    max_current{3.3f / config.shunt_resistor}, // Assuming 3.3V reference voltage
    max_torque{config.max_torque},
    noise{config.noise},
    gen(rd()),
    noise_dist(0.0f, noise) {
    this->calibrate();
}

template <uint8_t num_of_sensors>
void TTorqueSensors<num_of_sensors>::calibrate() {
    for (uint8_t i = 0; i < num_of_sensors; i++) {
        this->base_reading[i] = 0.0f;
        this->filtered_readings[i] = 0.0f;
        this->simulated_torque[i] = 0.0f;
    }
}

template <uint8_t num_of_sensors>
void TTorqueSensors<num_of_sensors>::update() {
    for (uint8_t i = 0; i < num_of_sensors; i++) {
        // Add noise to the simulated torque
        float noisy_torque = this->simulated_torque[i] + this->noise_dist(this->gen);
        
        // Simple low-pass filter
        const float alpha = 0.1f; // Filter coefficient
        this->filtered_readings[i] = alpha * noisy_torque + (1.0f - alpha) * this->filtered_readings[i];
    }
}

template <uint8_t num_of_sensors>
float TTorqueSensors<num_of_sensors>::get_torque(uint8_t sensor_index) const {
    return this->filtered_readings.at(sensor_index);
}

template <uint8_t num_of_sensors>
float TTorqueSensors<num_of_sensors>::get_torque_raw(uint8_t sensor_index) const {
    return this->simulated_torque.at(sensor_index);
}

template <uint8_t num_of_sensors>
float TTorqueSensors<num_of_sensors>::get_current(uint8_t sensor_index) const {
    return this->filtered_readings.at(sensor_index) / this->max_torque * this->max_current;
}

template <uint8_t num_of_sensors>
float TTorqueSensors<num_of_sensors>::get_current_raw(uint8_t sensor_index) const {
    return this->simulated_torque.at(sensor_index) / this->max_torque * this->max_current;
}

template <uint8_t num_of_sensors>
float TTorqueSensors<num_of_sensors>::get_adc_reading(uint8_t sensor_index) const {
    return this->filtered_readings.at(sensor_index) / this->max_torque;
}

template <uint8_t num_of_sensors>
void TTorqueSensors<num_of_sensors>::set_torque(uint8_t sensor_index, float torque) {
    if (sensor_index < num_of_sensors) {
        this->simulated_torque[sensor_index] = std::clamp(torque, -this->max_torque, this->max_torque);
    }
}

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_TORQUE_SENSORS_CPP 
