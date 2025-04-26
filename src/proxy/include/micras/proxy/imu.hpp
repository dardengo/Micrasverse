#ifndef MICRAS_PROXY_IMU_HPP
#define MICRAS_PROXY_IMU_HPP

#include <array>
#include <cstdint>
#include "box2d/box2d.h"
#include "physics/box2d_micrasbody.hpp"

namespace micras::proxy {

class Imu {
public:
    struct Config {
        micrasverse::physics::Box2DMicrasBody* micrasBody = nullptr;
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

    bool was_initialized() const;

private:
    micrasverse::physics::Box2DMicrasBody* micrasBody;
    b2BodyId bodyId;
    float gyroscope_noise;
    float accelerometer_noise;
    b2Vec2 current_linear_velocity;
    b2Vec2 previous_linear_velocity;

    std::array<float, 3> angular_velocity{};
    std::array<float, 3> linear_acceleration{};
    bool calibrated{false};
    bool initialized{true};
    static constexpr float radps_to_mdps{180000.0F / B2_PI};
    static constexpr float mps2_to_mg{1.0F / 0.00980665F};
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_IMU_HPP 
