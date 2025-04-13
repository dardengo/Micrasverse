#ifndef MICRAS_PROXY_BATTERY_HPP
#define MICRAS_PROXY_BATTERY_HPP

#include <cstdint>
#include "box2d/box2d.h"

namespace micras::proxy {

class Battery {
public:
    struct Config {
        float voltage;
        float voltage_divider;
        float filter_cutoff;
        float noise;
    };

    explicit Battery(const Config& config, const b2BodyId bodyId);

    void update();

    float get_voltage() const;

    float get_voltage_raw() const;

    float get_adc_reading() const;

private:
    b2BodyId bodyId;
    float voltage;
    float voltage_divider;
    float noise;
    float raw_reading{0.0f};
    float filtered_reading{0.0f};
    float max_voltage;
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_BATTERY_HPP 
