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

// Forward declaration
namespace micras::proxy {

template <uint8_t num_of_sensors>
class TWallSensors {
public:
    struct Config {
        micrasverse::physics::Box2DMicrasBody* micrasBody;
        float                                  uncertainty;
        std::array<float, num_of_sensors>      base_readings;
        float                                  K;
        float                                  max_adc_reading;
        float                                  max_distance;
        float                                  filter_cutoff;
    };

    explicit TWallSensors(const Config& config);

    ~TWallSensors();

    void turn_on();

    void turn_off();

    void update();

    // Methods matching MicrasFirmware interface
    bool get_wall(uint8_t sensor_index, bool disturbed = false) const;

    float get_reading(uint8_t sensor_index) const;

    float get_adc_reading(uint8_t sensor_index) const;

    float get_sensor_error(uint8_t sensor_index) const;

    void calibrate_front_wall();

    void calibrate_left_wall();

    void calibrate_right_wall();

    /**
     * @brief Calibrate a wall sensor base reading.
     */
    void calibrate_sensor(uint8_t sensor_index);

private:
    micrasverse::physics::Box2DMicrasBody*              micrasBody;
    float                                               uncertainty;
    std::array<float, num_of_sensors>                   base_readings{};
    float                                               K;
    float                                               max_adc_reading;
    float                                               max_distance;
    std::array<core::ButterworthFilter, num_of_sensors> filters;
};

}  // namespace micras::proxy

// Include the template implementation
#include "../src/wall_sensors.cpp"

#endif  // MICRAS_PROXY_WALL_SENSORS_HPP
