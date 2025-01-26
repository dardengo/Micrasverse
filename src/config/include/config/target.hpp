#ifndef MICRAS_TARGET_HPP
#define MICRAS_TARGET_HPP

#include "proxy/wall_sensors.hpp"
#include "proxy/argb.hpp"

namespace micrasverse {

namespace proxy{
    using WallSensors = micrasverse::proxy::TWallSensors<4>;
    using Argb = micrasverse::proxy::TArgb<1>;
}

const proxy::WallSensors::Config wall_sensors_config = {
    .uncertainty = 0.35F,
    .wall_threshold =
        {
            0.42418F,
            0.0961F,
            0.09384F,
            0.31986F,
        },
    .free_threshold =
        {
            0.39012F,
            0.0881F,
            0.08916F,
            0.28704F,
        },
};

const proxy::Argb::Config argb_config = {
    .max_brightness = 100.0F,
};

} // namespace micras

#endif // MICRAS_TARGET_HPP
