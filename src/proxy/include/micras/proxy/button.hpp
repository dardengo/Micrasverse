#pragma once

#include <cstdint>
#include "box2d/box2d.h"
#include "micras/proxy/stopwatch.hpp"
#include <memory>

namespace micras::proxy {

class Button {
public:
    enum class Status {
        NO_PRESS,
        SHORT_PRESS,
        LONG_PRESS,
        EXTRA_LONG_PRESS
    };

    enum class PullResistor {
        PULL_UP,
        PULL_DOWN
    };

    struct Config {
        b2BodyId bodyId;
        b2WorldId worldId;
        PullResistor pull_resistor;
        uint32_t debounce_delay;  // ms
        uint32_t long_press_delay;  // ms
        uint32_t extra_long_press_delay;  // ms
        bool initial_state;
    };

    explicit Button(const Config& config);

    bool is_pressed();
    Status get_status();
    void set_state(bool state);

private:
    bool get_raw_reading() const;
    void update_state();
    bool is_rising_edge() const;
    bool is_falling_edge() const;

    b2BodyId bodyId;
    b2WorldId worldId;
    PullResistor pull_resistor;
    uint32_t debounce_delay;
    uint32_t long_press_delay;
    uint32_t extra_long_press_delay;
    bool simulated_state;
    bool last_state;
    bool current_state;
    bool debouncing;
    std::unique_ptr<Stopwatch> debounce_timer;
    std::unique_ptr<Stopwatch> press_timer;
};

} // namespace micras::proxy 
