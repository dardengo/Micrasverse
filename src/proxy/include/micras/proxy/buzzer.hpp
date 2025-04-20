#ifndef MICRAS_PROXY_BUZZER_HPP
#define MICRAS_PROXY_BUZZER_HPP

#include "micras/core/types.hpp"
#include "physics/box2d_micrasbody.hpp"

#include <cstdint>
#include "micras/proxy/stopwatch.hpp"
#include <memory>

namespace micras::proxy {

class Buzzer {
public:
    struct Config {
        micrasverse::physics::Box2DMicrasBody* micrasBody = nullptr;
        uint8_t volume;
    };

    explicit Buzzer(const Config& config);

    void play(uint32_t frequency, uint32_t duration = 0);

    void set_frequency(float frequency);

    void set_duration(uint32_t duration);

    void update();

    void stop();

    void wait(uint32_t interval);

    bool is_playing() const;

private:
    micrasverse::physics::Box2DMicrasBody* micrasBody;
    float volume;
    bool playing{false};
    uint32_t duration{0};
    float start_time{0.0f};
    uint32_t current_frequency{0};
    std::unique_ptr<Stopwatch> tone_timer;
    std::unique_ptr<Stopwatch> wait_timer;
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_BUZZER_HPP 
