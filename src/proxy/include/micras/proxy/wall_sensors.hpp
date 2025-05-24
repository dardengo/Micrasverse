#ifndef MICRAS_PROXY_WALL_SENSORS_HPP
#define MICRAS_PROXY_WALL_SENSORS_HPP

#include "physics/box2d_micrasbody.hpp"
#include "physics/box2d_distance_sensor.hpp"
#include "micras/core/types.hpp"
#include "micras/core/butterworth_filter.hpp"

#include <array>
#include <cstdint>
#include <vector>
#include <memory>

namespace micras::proxy {

template <uint8_t num_of_sensors>
class TWallSensors {
public:
    struct Config {
        micrasverse::physics::Box2DMicrasBody* micrasBody;
        float                                  uncertainty;
        std::array<float, num_of_sensors>      base_readings;
        float                                  max_sensor_reading;
        float                                  min_sensor_reading;
        float                                  max_sensor_distance;
        float                                  filter_cutoff;
    };

    explicit TWallSensors(const Config& config);

    ~TWallSensors();

    void turn_on();

    void turn_off();

    void update();

    bool get_wall(uint8_t sensor_index, bool disturbed = false) const;

    float get_reading(uint8_t sensor_index) const;

    float get_adc_reading(uint8_t sensor_index) const;

    float get_sensor_error(uint8_t sensor_index) const;

    void calibrate_front_wall();

    void calibrate_left_wall();

    void calibrate_right_wall();

    void calibrate_sensor(uint8_t sensor_index);

private:
    micrasverse::physics::Box2DMicrasBody*              micrasBody;
    float                                               uncertainty;
    std::array<float, num_of_sensors>                   base_readings{};
    float                                               max_sensor_reading;
    float                                               c;
    std::array<core::ButterworthFilter, num_of_sensors> filters;
};

}  // namespace micras::proxy

#include "../src/wall_sensors.cpp"

#endif  // MICRAS_PROXY_WALL_SENSORS_HPP
