#include "micras/proxy/battery.hpp"
#include <random>
#include <algorithm>

namespace micras::proxy {

Battery::Battery(const Config& config) :
    micrasBody{config.micrasBody},
    voltage{config.voltage},
    voltage_divider{config.voltage_divider},
    noise{config.noise},
    max_voltage{config.voltage * config.voltage_divider} { }

void Battery::update() {
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::normal_distribution<float> noise_dist(0.0f, noise);

    float noisy_voltage = voltage + noise_dist(gen);

    raw_reading = std::clamp(noisy_voltage / max_voltage, 0.0f, 1.0f);

    const float alpha = 0.1f;
    filtered_reading = alpha * raw_reading + (1.0f - alpha) * filtered_reading;
}

float Battery::get_voltage() const {
    return filtered_reading * max_voltage;
}

float Battery::get_voltage_raw() const {
    return raw_reading * max_voltage;
}

float Battery::get_adc_reading() const {
    return raw_reading;
}

}  // namespace micras::proxy
