#ifndef MICRAS_PROXY_BATTERY_HPP
#define MICRAS_PROXY_BATTERY_HPP

#include <cstdint>
#include "physics/box2d_micrasbody.hpp"

namespace micras::proxy {

class Battery {
public:
    struct Config {
        micrasverse::physics::Box2DMicrasBody* micrasBody = nullptr;
        float voltage;
        float voltage_divider;
        float filter_cutoff;
        float noise;
    };

    explicit Battery(const Config& config);

    void update();

    float get_voltage() const;

    float get_voltage_raw() const;

    float get_adc_reading() const;

private:
    micrasverse::physics::Box2DMicrasBody* micrasBody;
    float voltage;
    float voltage_divider;
    float noise;
    float raw_reading{0.0f};
    float filtered_reading{0.0f};
    float max_voltage;
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_BATTERY_HPP 
