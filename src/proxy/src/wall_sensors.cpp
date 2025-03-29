#ifndef MICRAS_PROXY_WALL_SENSORS_CPP
#define MICRAS_PROXY_WALL_SENSORS_CPP

#include "physics/distancesensor.hpp"
#include "core/types.hpp"
#include "config/target.hpp"

namespace micras::proxy {

template <uint8_t num_of_sensors>
TWallSensors<num_of_sensors>::TWallSensors(const Config& config, const b2BodyId bodyId) :
    bodyId{bodyId},
    uncertainty{config.uncertainty},
    wall_threshold{config.wall_threshold},
    free_space_threshold{config.free_threshold} {

    this->attach_sensor({ 0.028f, 0.045f},  B2_PI / 2.0f);
    this->attach_sensor({-0.028f, 0.045f},  B2_PI / 2.0f);
    this->attach_sensor({ 0.009f,  0.049f}, B2_PI / 6.0f);
    this->attach_sensor({-0.009f,  0.049f}, 5.0f * B2_PI / 6.0f);
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
    for (auto& sensor : this->sensors) {
        sensor.getReading();
    }
}

template <uint8_t num_of_sensors>
core::Observation TWallSensors<num_of_sensors>::get_observation(uint8_t sensor_index) const {
    const float reading = this->sensors.at(sensor_index).reading;

    if (reading > this->wall_threshold.at(sensor_index)) {
        return core::Observation::WALL;
    }

    if (reading < this->free_space_threshold.at(sensor_index)) {
        return core::Observation::FREE_SPACE;
    }

    return core::Observation::UNKNOWN;
}

template <uint8_t num_of_sensors>
float TWallSensors<num_of_sensors>::get_reading(uint8_t sensor_index) {
    return this->sensors.at(sensor_index).getReading();
}

template <uint8_t num_of_sensors>
float TWallSensors<num_of_sensors>::get_adc_reading(uint8_t sensor_index) const {
    return 0.0F;
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_front_wall() {
    this->wall_calibration_measure[0] = this->get_reading(0);
    this->wall_calibration_measure[3] = this->get_reading(3);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_left_wall() {
    this->wall_calibration_measure[1] = this->get_reading(1);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_right_wall() {
    this->wall_calibration_measure[2] = this->get_reading(2);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_front_free_space() {
    this->free_space_calibration_measure[0] = this->get_reading(0);
    this->free_space_calibration_measure[3] = this->get_reading(3);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_left_free_space() {
    this->free_space_calibration_measure[1] = this->get_reading(1);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::calibrate_right_free_space() {
    this->free_space_calibration_measure[2] = this->get_reading(2);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::update_thresholds() {
    for (uint8_t i = 0; i < num_of_sensors; i++) {
        float mean_value = (this->wall_calibration_measure.at(i) + this->free_space_calibration_measure.at(i)) / 2.0F;

        float unknown_delta = this->uncertainty *
                              (this->wall_calibration_measure.at(i) - this->free_space_calibration_measure.at(i)) /
                              2.0F;

        this->wall_threshold.at(i) = mean_value + unknown_delta;
        this->free_space_threshold.at(i) = mean_value - unknown_delta;
    }
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::attach_sensor(const b2Vec2 localPosition, const float angle) {
    physics::DistanceSensor sensor = physics::DistanceSensor(this->bodyId, localPosition, angle);
    this->sensors.push_back(sensor);
}

template <uint8_t num_of_sensors>
std::vector<physics::DistanceSensor>& TWallSensors<num_of_sensors>::get_sensors() {
    return this->sensors;
}

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_WALL_SENSORS_CPP
