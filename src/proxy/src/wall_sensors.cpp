#ifndef MICRAS_PROXY_WALL_SENSORS_CPP
#define MICRAS_PROXY_WALL_SENSORS_CPP

#include "micras/proxy/wall_sensors.hpp"
#include "micras/core/types.hpp"
#include "micras/core/utils.hpp"

namespace micras::proxy {

template <uint8_t num_of_sensors>
TWallSensors<num_of_sensors>::TWallSensors(const typename TWallSensors<num_of_sensors>::Config& config) :
    micrasBody{config.micrasBody},
    uncertainty{config.uncertainty},
    base_readings{config.base_readings},
    K{config.K},
    max_adc_reading{config.max_adc_reading},
    max_distance{config.max_distance},
    filters{core::make_array<core::ButterworthFilter, num_of_sensors>(config.filter_cutoff)} { }

template <uint8_t num_of_sensors>
TWallSensors<num_of_sensors>::~TWallSensors() { }

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::turn_on() { }

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::turn_off() { }

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::update() {
    for (uint8_t i = 0; i < num_of_sensors; i++) {
        this->filters[i].update(this->get_adc_reading(i));
    }
}

template <uint8_t num_of_sensors>
bool TWallSensors<num_of_sensors>::get_wall(uint8_t sensor_index, bool disturbed) const {
    float reading = this->get_reading(sensor_index);
    float threshold = this->base_readings.at(sensor_index) * uncertainty;

    return reading > threshold;
}

template <uint8_t num_of_sensors>
float TWallSensors<num_of_sensors>::get_reading(uint8_t sensor_index) const {
    return this->filters.at(sensor_index).get_last();
}

template <uint8_t num_of_sensors>
float TWallSensors<num_of_sensors>::get_adc_reading(uint8_t sensor_index) const {
    float distance = micrasBody->getDistanceSensor(sensor_index).getReading();
    if (distance >= max_distance) {
        return 0;
    }

    float reading = (K * (1.0f - (distance / max_distance)));

    return reading;
}

template <uint8_t num_of_sensors>
float TWallSensors<num_of_sensors>::get_sensor_error(uint8_t sensor_index) const {
    return this->get_reading(sensor_index) - base_readings.at(sensor_index);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_front_wall() {
    this->base_readings.at(0) = this->get_reading(0);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_left_wall() {
    this->base_readings.at(1) = this->get_reading(1);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_right_wall() {
    this->base_readings.at(2) = this->get_reading(2);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_sensor(uint8_t sensor_index) {
    this->base_readings.at(sensor_index) = this->get_reading(sensor_index);
}

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_WALL_SENSORS_CPP
