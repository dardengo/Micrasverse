#ifndef MICRAS_PROXY_DIP_SWITCH_CPP
#define MICRAS_PROXY_DIP_SWITCH_CPP

#include "micras/proxy/dip_switch.hpp"

namespace micras::proxy {

template <uint8_t num_of_switches>
TDipSwitch<num_of_switches>::TDipSwitch(const Config& config) :
    micrasBody{config.micrasBody},
    switch_states{config.initial_states} { }

template <uint8_t num_of_switches>
bool TDipSwitch<num_of_switches>::get_switch_state(uint8_t switch_index) const {
    return switch_states.at(switch_index);
}

template <uint8_t num_of_switches>
uint8_t TDipSwitch<num_of_switches>::get_switches_value() const {
    uint8_t switches_value = 0;

    for (uint8_t i = 0; i < num_of_switches; i++) {
        switches_value |= (switch_states[i] << i);
    }

    return switches_value;
}

template <uint8_t num_of_switches>
void TDipSwitch<num_of_switches>::set_switch_state(uint8_t switch_index, bool state) {
    if (switch_index < num_of_switches) {
        switch_states[switch_index] = state;
    }
}

template <uint8_t num_of_switches>
void TDipSwitch<num_of_switches>::setState(uint8_t value) {
    for (uint8_t i = 0; i < num_of_switches; i++) {
        switch_states[i] = (value & (1 << i)) != 0;
    }
}

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_DIP_SWITCH_CPP 
