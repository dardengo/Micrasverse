#ifndef MICRAS_PROXY_TORQUE_SENSORS_HPP
#define MICRAS_PROXY_TORQUE_SENSORS_HPP

#include <array>
#include <cstdint>
#include <random>
#include "physics/box2d_micrasbody.hpp"

namespace micras::proxy {

/**
 * @brief Class for acquiring torque sensors data in the simulation.
 */
template <uint8_t num_of_sensors>
class TTorqueSensors {
public:
    /**
     * @brief Configuration struct for torque sensors.
     */
    struct Config {
        micrasverse::physics::Box2DMicrasBody* micrasBody = nullptr;
        float shunt_resistor;
        float max_torque;
        float max_current;
        float filter_cutoff;
        float noise;
    };

    /**
     * @brief Construct a new TorqueSensors object.
     *
     * @param config Configuration for the torque sensors.
     */
    explicit TTorqueSensors(const Config& config);

    /**
     * @brief Calibrate the torque sensors.
     */
    void calibrate();

    /**
     * @brief Update the torque sensors readings.
     */
    void update();

    /**
     * @brief Get the torque from the sensor.
     *
     * @param sensor_index Index of the sensor.
     * @return Torque reading from the sensor in N*m.
     */
    float get_torque(uint8_t sensor_index) const;

    /**
     * @brief Get the raw torque from the sensor without filtering.
     *
     * @param sensor_index Index of the sensor.
     * @return Raw torque reading from the sensor.
     */
    float get_torque_raw(uint8_t sensor_index) const;

    /**
     * @brief Get the electric current through the sensor.
     *
     * @param sensor_index Index of the sensor.
     * @return Current reading from the sensor in amps.
     */
    float get_current(uint8_t sensor_index) const;

    /**
     * @brief Get the raw electric current through the sensor without filtering.
     *
     * @param sensor_index Index of the sensor.
     * @return Raw current reading from the sensor in amps.
     */
    float get_current_raw(uint8_t sensor_index) const;

    /**
     * @brief Get the ADC reading from the sensor.
     *
     * @param sensor_index Index of the sensor.
     * @return Adc reading from the sensor from 0 to 1.
     */
    float get_adc_reading(uint8_t sensor_index) const;

    /**
     * @brief Set the simulated torque for a sensor.
     *
     * @param sensor_index Index of the sensor.
     * @param torque Torque to set in N*m.
     */
    void set_torque(uint8_t sensor_index, float torque);

private:
    micrasverse::physics::Box2DMicrasBody* micrasBody;
    float shunt_resistor;
    float max_torque;
    float max_current;
    float filter_cutoff;
    float noise;
    std::array<float, num_of_sensors> base_reading{};
    std::array<float, num_of_sensors> simulated_torque{};
    std::array<float, num_of_sensors> filtered_readings{};
    std::random_device rd;
    std::mt19937 gen;
    std::normal_distribution<float> noise_dist;
};

}  // namespace micras::proxy

#include "../src/torque_sensors.cpp"  // NOLINT(bugprone-suspicious-include, misc-header-include-cycle)

#endif  // MICRAS_PROXY_TORQUE_SENSORS_HPP 
