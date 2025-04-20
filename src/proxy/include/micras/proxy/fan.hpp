#ifndef MICRAS_PROXY_FAN_HPP
#define MICRAS_PROXY_FAN_HPP

#include <cstdint>
#include "micras/proxy/stopwatch.hpp"
#include <memory>
#include "physics/box2d_micrasbody.hpp"

namespace micras::proxy {

class Fan {
public:
    enum RotationDirection : uint8_t {
        FORWARD,
        BACKWARDS
    };

    struct Config {
        micrasverse::physics::Box2DMicrasBody* micrasBody = nullptr;
        float max_acceleration;  // Maximum acceleration in speed units per second
        float max_speed;         // Maximum speed in speed units
    };

    explicit Fan(const Config& config);

    void enable();

    void disable();

    void set_speed(float speed);

    float update();

    void stop();

    float get_speed() const;

    void set_direction(RotationDirection direction);

private:
    micrasverse::physics::Box2DMicrasBody* micrasBody;
    float max_acceleration;
    float max_speed;
    float current_speed = 0.0f;
    float target_speed = 0.0f;
    bool enabled = false;
    float last_update_time{0.0f};
    RotationDirection current_direction = RotationDirection::FORWARD;
    std::unique_ptr<Stopwatch> stopwatch;
};

}  // namespace micras::proxy
#endif  // MICRAS_PROXY_FAN_HPP 

