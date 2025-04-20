#ifndef MICRAS_PROXY_LED_HPP
#define MICRAS_PROXY_LED_HPP

#include "micras/core/types.hpp"
#include "physics/box2d_micrasbody.hpp"

#include <cstdint>

namespace micras::proxy {

class Led {
public:
    
    struct Config {
        micrasverse::physics::Box2DMicrasBody* micrasBody;
        bool initial_state;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };
    
    explicit Led(const Config& config);
    
    void turn_on();
    
    void turn_off();
    
    void toggle();

    void setState(bool state);

    bool is_on() const;

    uint8_t get_red() const;

    uint8_t get_green() const;

    uint8_t get_blue() const;

private:
    void updateColor();
    micrasverse::physics::Box2DMicrasBody* micrasBody;
    uint8_t ledIndex;
    bool state{false};
    uint8_t red{255};
    uint8_t green{255};
    uint8_t blue{255};
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_LED_HPP 

