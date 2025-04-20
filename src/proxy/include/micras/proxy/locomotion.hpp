#ifndef MICRAS_PROXY_LOCOMOTION_HPP
#define MICRAS_PROXY_LOCOMOTION_HPP

#include "physics/box2d_micrasbody.hpp"

namespace micras::proxy {

class Locomotion {
public:
    struct Config {
        micrasverse::physics::Box2DMicrasBody* micrasBody = nullptr;
    };

    explicit Locomotion(const Config& config);

    void enable();

    void disable();

    void update(float deltaTime, bool isFanOn);

    void set_wheel_command(float left_command, float right_command);

    void set_command(float linear, float angular);

    void stop();

private:
    micrasverse::physics::Box2DMicrasBody* micrasBody;
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_LOCOMOTION_HPP
