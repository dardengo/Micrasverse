#ifndef MICRAS_PROXY_DIP_SWITCH_HPP
#define MICRAS_PROXY_DIP_SWITCH_HPP

#include "micras/core/types.hpp"
#include "physics/box2d_micrasbody.hpp"

#include <array>
#include <cstdint>

namespace micras::proxy {

template <uint8_t num_of_switches>
class TDipSwitch {
public:

    struct Config {
        micrasverse::physics::Box2DMicrasBody* micrasBody = nullptr;
        std::array<bool, num_of_switches> initial_states;
    };

    explicit TDipSwitch(const Config& config);

    bool get_switch_state(uint8_t switch_index) const;

    uint8_t get_switches_value() const;

    void set_switch_state(uint8_t switch_index, bool state);

    void setState(uint8_t value);

private:
    micrasverse::physics::Box2DMicrasBody* micrasBody;
    std::array<bool, num_of_switches> switch_states;
};

}  // namespace micras::proxy

#include "../src/dip_switch.cpp"  // NOLINT(bugprone-suspicious-include, misc-header-include-cycle)

#endif  // MICRAS_PROXY_DIP_SWITCH_HPP 
