#ifndef MICRAS_PROXY_WALL_SENSORS_TPP
#define MICRAS_PROXY_WALL_SENSORS_TPP

// Note: We only need the adapter declaration, not its implementation
#include "micras/proxy/box2d_sensor_adapter.hpp"
#include "micras/proxy/wall_sensors.hpp"
// The path is relative to the include directories in CMakeLists.txt
#include "micras/core/types.hpp"

namespace micras::proxy {

template <uint8_t num_of_sensors>
TWallSensors<num_of_sensors>::TWallSensors(const typename TWallSensors<num_of_sensors>::Config& config) :
    uncertainty{config.uncertainty},
    base_readings{config.base_readings},
    K{config.K},
    max_adc_reading{config.max_adc_reading},
    max_distance{config.max_distance} {
    // Initialize the wall sensors with adapters
    for (uint8_t i = 0; i < num_of_sensors; i++) {
        sensors.push_back(std::make_unique<Box2DSensorAdapter>(config.micrasBody, i));
    }
}

template <uint8_t num_of_sensors>
TWallSensors<num_of_sensors>::~TWallSensors() {
    // No resources to clean up since we're using unique_ptr
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::turn_on() {
    // No physical hardware to turn on in the simulation
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::turn_off() {
    // No physical hardware to turn off in the simulation
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::update() {
    // The sensors are already updated in Box2DMicrasBody's update method
}

template <uint8_t num_of_sensors>
bool TWallSensors<num_of_sensors>::get_wall(uint8_t sensor_index, bool disturbed) const {   
    return this->get_reading(sensor_index) > this->base_readings.at(sensor_index)*uncertainty;
}

template <uint8_t num_of_sensors>
micras::core::Observation TWallSensors<num_of_sensors>::get_observation() const {
    if (this->get_wall(0) && this->get_wall(3)) {
        return {this->get_wall(1, true), true, this->get_wall(2, true)};
    }

    return {this->get_wall(1), false, this->get_wall(2)};
}

template <uint8_t num_of_sensors>
float TWallSensors<num_of_sensors>::get_reading(uint8_t sensor_index) const {
    return get_adc_reading(sensor_index);
}

template <uint8_t num_of_sensors>
float TWallSensors<num_of_sensors>::get_adc_reading(uint8_t sensor_index) const {
    // Use the sensor adapter interface to get the reading
    float distance = sensors.at(sensor_index)->getReading();
    if (distance >= max_distance) {
        return 0;
    }
    
    return (K * (1.0f - (distance / max_distance)));
}

template <uint8_t num_of_sensors>
float TWallSensors<num_of_sensors>::get_sensor_error(uint8_t sensor_index) const {
    // In MicrasFirmware, this returns the difference between current reading and base (calibrated) reading
    return this->get_reading(sensor_index) - base_readings.at(sensor_index);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_front_wall() {
    this->base_readings.at(2) = this->get_reading(2);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_left_wall() {
    this->base_readings.at(0) = this->get_reading(0);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_right_wall() {
    this->base_readings.at(1) = this->get_reading(1);
}

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_WALL_SENSORS_TPP 
