#ifndef MICRAS_PROXY_WALL_SENSORS_HPP
#define MICRAS_PROXY_WALL_SENSORS_HPP

#include "micras/core/types.hpp"
#include "micras/proxy/i_distance_sensor.hpp"

#include <array>
#include <cstdint>
#include <vector>
#include <memory>

// Forward declaration
namespace micrasverse::physics {
class Box2DMicrasBody;
}

namespace micras::proxy {

template <uint8_t num_of_sensors>
class TWallSensors {
public:
    struct Config {
        micrasverse::physics::Box2DMicrasBody* micrasBody;
        float uncertainty;
        std::array<float, num_of_sensors> base_readings;
        float K;
        float max_adc_reading;
        float max_distance;
    };

    explicit TWallSensors(const Config& config);

    ~TWallSensors();

    void turn_on();

    void turn_off();

    void update();

    // Methods matching MicrasFirmware interface
    bool get_wall(uint8_t sensor_index, bool disturbed = false) const;
    
    micras::core::Observation get_observation() const;
    
    float get_reading(uint8_t sensor_index) const;
    
    float get_adc_reading(uint8_t sensor_index) const;
    
    float get_sensor_error(uint8_t sensor_index) const;
    
    void calibrate_front_wall();
    
    void calibrate_left_wall();
    
    void calibrate_right_wall();

    // Additional methods for Micrasverse (for backward compatibility)
    void calibrate_front_free_space();

    void calibrate_left_free_space();

    void calibrate_right_free_space();

    void update_thresholds();

private:
    std::vector<std::unique_ptr<IDistanceSensor>> sensors;
    float uncertainty;
    std::array<float, num_of_sensors> base_readings{};
    float K;
    float max_adc_reading;
    float max_distance;
};

}  // namespace micras::proxy

// Include the template implementation
#include "../src/wall_sensors.tpp"

#endif  // MICRAS_PROXY_WALL_SENSORS_HPP
