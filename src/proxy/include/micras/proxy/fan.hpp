#ifndef MICRAS_PROXY_FAN_HPP
#define MICRAS_PROXY_FAN_HPP

#include <cstdint>
#include <memory>
#include "physics/box2d_micrasbody.hpp"

namespace micras::proxy {

class Fan {
public:
    enum RotationDirection : uint8_t {
        FORWARD = 0,
        BACKWARDS = 1
    };

    struct Config {
        micrasverse::physics::Box2DMicrasBody* micrasBody = nullptr;
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
    float                                  current_speed = 0.0f;
    bool                                   enabled = false;
    float                                  last_update_time{0.0f};
    RotationDirection                      current_direction = RotationDirection::FORWARD;
};

}  // namespace micras::proxy
#endif  // MICRAS_PROXY_FAN_HPP
