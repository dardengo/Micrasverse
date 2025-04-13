#ifndef MICRAS_TARGET_HPP
#define MICRAS_TARGET_HPP

#include "micras/proxy/wall_sensors.hpp"
#include "micras/proxy/argb.hpp"

namespace micras::proxy{
    using WallSensors = micras::proxy::TWallSensors<4>;
    using Argb = micras::proxy::TArgb<1>;


const WallSensors::Config wall_sensors_config = {
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

const Argb::Config argb_config = {
    .max_brightness = 100.0F,
};

} // namespace micras::proxy

#endif // MICRAS_TARGET_HPP
