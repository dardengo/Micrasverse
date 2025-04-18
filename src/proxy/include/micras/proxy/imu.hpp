#ifndef MICRAS_PROXY_IMU_HPP
#define MICRAS_PROXY_IMU_HPP

#include <array>
#include <cstdint>
#include <numbers>
#include "box2d/box2d.h"

namespace micras::proxy {

class Imu {
public:
    struct Config {
        b2BodyId bodyId;
        float gyroscope_noise;
        float accelerometer_noise;
    };

    enum Axis : uint8_t {
        X,
        Y,
        Z
    };

    explicit Imu(const Config& config);

    bool check_whoami();

    void update();

    float get_angular_velocity(Axis axis) const;

    float get_linear_acceleration(Axis axis) const;

    void calibrate();

private:
    b2BodyId bodyId;
    float gyroscope_noise;
    float accelerometer_noise;

    std::array<float, 3> angular_velocity{};
    std::array<float, 3> linear_acceleration{};
    bool calibrated{false};

    static constexpr float radps_to_mdps{180000.0F / std::numbers::pi_v<float>};
    static constexpr float mps2_to_mg{1.0F / 0.00980665F};
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_IMU_HPP 
