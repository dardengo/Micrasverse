#ifndef MICRAS_PROXY_WALL_SENSORS_CPP
#define MICRAS_PROXY_WALL_SENSORS_CPP

#include "micras/proxy/wall_sensors.hpp"
#include "micras/core/types.hpp"
#include "physics/i_distance_sensor.hpp"
#include "physics/box2d_distance_sensor.hpp"

namespace micras::proxy {

template <uint8_t num_of_sensors>
TWallSensors<num_of_sensors>::TWallSensors(const Config& config) :
    bodyId{config.bodyId},
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
        sensor.update();
    }
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
    return this->sensors.at(sensor_index).getReading();
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
    this->wall_calibration_measure.at(4) = this->get_reading(4);
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
    this->free_space_calibration_measure.at(4) = this->get_reading(4);
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::update_thresholds() {
    for (uint8_t i = 0; i < num_of_sensors; i++) {
        this->wall_threshold.at(i) = this->wall_calibration_measure.at(i) * (1.0F - this->uncertainty);
        this->free_space_threshold.at(i) = this->free_space_calibration_measure.at(i) * (1.0F + this->uncertainty);
    }
}

template <uint8_t num_of_sensors>
void TWallSensors<num_of_sensors>::attach_sensor(const b2Vec2 localPosition, const float angle) {
    // Get the world ID from the body ID
    b2WorldId worldId = b2Body_GetWorld(bodyId);
    
    // Convert angle to direction vector
    micrasverse::types::Vec2 direction = {
        std::cos(angle),
        std::sin(angle)
    };
    
    // Use a reasonable default max distance (1 meter)
    const float maxDistance = 1.0f;
    
    // Create the sensor with the world ID, body ID, local position, direction, and max distance
    this->sensors.push_back(micrasverse::physics::Box2DDistanceSensor(
        worldId, 
        bodyId, 
        micrasverse::types::Vec2{localPosition.x, localPosition.y}, 
        direction, 
        maxDistance
    ));
}

template <uint8_t num_of_sensors>
std::vector<micrasverse::physics::Box2DDistanceSensor>& TWallSensors<num_of_sensors>::get_sensors() {
    return this->sensors;
}

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_WALL_SENSORS_CPP
