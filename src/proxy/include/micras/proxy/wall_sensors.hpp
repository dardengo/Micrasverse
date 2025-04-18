#ifndef MICRAS_PROXY_WALL_SENSORS_HPP
#define MICRAS_PROXY_WALL_SENSORS_HPP

#include "physics/box2d_distance_sensor.hpp"
#include "micrasverse_core/types.hpp"
#include "micras/core/types.hpp"

#include "box2d/box2d.h"

#include <array>
#include <cstdint>
#include <vector>

namespace micras::proxy {

template <uint8_t num_of_sensors>
class TWallSensors {
public:
    struct Config {
        b2BodyId bodyId;
        float uncertainty;
        std::array<float, num_of_sensors> wall_threshold;
        std::array<float, num_of_sensors> free_threshold;
    };

    explicit TWallSensors(const Config& config);

    ~TWallSensors();

    void turn_on();

    void turn_off();

    void update();

    micras::core::Observation get_observation(uint8_t sensor_index) const;

    float get_reading(uint8_t sensor_index) const;

    float get_adc_reading(uint8_t sensor_index) const;

    void calibrate_front_wall();

    void calibrate_left_wall();

    void calibrate_right_wall();

    void calibrate_front_free_space();

    void calibrate_left_free_space();

    void calibrate_right_free_space();

    void update_thresholds();

    void attach_sensor(const b2Vec2 localPosition, const float angle);

    std::vector<micrasverse::physics::Box2DDistanceSensor>& get_sensors();

private:
    std::vector<micrasverse::physics::Box2DDistanceSensor> sensors;
    b2BodyId bodyId;
    
    float uncertainty;

    std::array<float, num_of_sensors> wall_calibration_measure;

    std::array<float, num_of_sensors> free_space_calibration_measure;

    std::array<float, num_of_sensors> wall_threshold;

    std::array<float, num_of_sensors> free_space_threshold;

};

}  // namespace micras::proxy

#include "../src/wall_sensors.cpp"

#endif  // MICRAS_PROXY_WALL_SENSORS_HPP
