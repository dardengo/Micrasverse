#ifndef MICRAS_PROXY_WALL_SENSORS_TPP
#define MICRAS_PROXY_WALL_SENSORS_TPP

// Note: We only need the adapter declaration, not its implementation
#include "micras/proxy/box2d_sensor_adapter.hpp"
// We don't need to include box2d_micrasbody.hpp here anymore

namespace micras::proxy {

template <uint8_t num_of_sensors>
TWallSensors<num_of_sensors>::TWallSensors(const Config& config) :
    uncertainty{config.uncertainty},
    wall_threshold{config.wall_threshold},
    free_space_threshold{config.free_threshold} {
    // Initialize the wall sensors with adapters
    for (uint8_t i = 0; i < num_of_sensors; i++) {
        sensors.push_back(std::make_unique<Box2DSensorAdapter>(config.micrasBody, i));
    }
    
    // Initialize calibration arrays
    wall_calibration_measure = {};
    free_space_calibration_measure = {};
}

template <uint8_t num_of_sensors>
TWallSensors<num_of_sensors>::~TWallSensors() {
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::turn_on() {
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::turn_off() {
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::update() {
    // The sensors are already updated in Box2DMicrasBody's update method
}

template <uint8_t num_of_sensors>
micras::core::Observation TWallSensors<num_of_sensors>::get_observation(uint8_t sensor_index) const {
    const float reading = this->get_reading(sensor_index);

    if (reading > this->wall_threshold.at(sensor_index)) {
        return micras::core::Observation::WALL;
    }

    if (reading < this->free_space_threshold.at(sensor_index)) {
        return micras::core::Observation::FREE_SPACE;
    }

    return micras::core::Observation::UNKNOWN;
}

template <uint8_t num_of_sensors>
float TWallSensors<num_of_sensors>::get_reading(uint8_t sensor_index) const {
    return get_adc_reading(sensor_index);
}

template <uint8_t num_of_sensors>
float TWallSensors<num_of_sensors>::get_adc_reading(uint8_t sensor_index) const {
    // Use the sensor adapter interface to get the reading
    return sensors.at(sensor_index)->getReading();
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_front_wall() {
    this->wall_calibration_measure.at(2) = this->get_reading(2);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_left_wall() {
    this->wall_calibration_measure.at(0) = this->get_reading(0);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_right_wall() {
    this->wall_calibration_measure.at(1) = this->get_reading(1);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_front_free_space() {
    this->free_space_calibration_measure.at(2) = this->get_reading(2);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_left_free_space() {
    this->free_space_calibration_measure.at(0) = this->get_reading(0);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_right_free_space() {
    this->free_space_calibration_measure.at(1) = this->get_reading(1);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::update_thresholds() {
    for (uint8_t i = 0; i < num_of_sensors; i++) {
        this->wall_threshold.at(i) = this->wall_calibration_measure.at(i) * (1.0F - this->uncertainty);
        this->free_space_threshold.at(i) = this->free_space_calibration_measure.at(i) * (1.0F + this->uncertainty);
    }
}

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_WALL_SENSORS_TPP 
