#ifndef MICRAS_PROXY_DIP_SWITCH_HPP
#define MICRAS_PROXY_DIP_SWITCH_HPP

#include <array>
#include <cstdint>
#include "box2d/box2d.h"

namespace micras::proxy {

/**
 * @brief Class for acquiring dip switch data in the simulation.
 */
template <uint8_t num_of_switches>
class TDipSwitch {
public:
    /**
     * @brief Configuration struct for the Dip Switch.
     */
    struct Config {
        b2BodyId bodyId;
        std::array<bool, num_of_switches> initial_states;
    };

    /**
     * @brief Construct a new Dip Switch object.
     *
     * @param config Configuration struct for the DipSwitch.
     */
    explicit TDipSwitch(const Config& config);

    /**
     * @brief Get the state of a switch.
     *
     * @param switch_index Index of the switch.
     * @return True if the switch is on, false otherwise.
     */
    bool get_switch_state(uint8_t switch_index) const;

    /**
     * @brief Get the value of all switches.
     *
     * @return Value of all switches.
     */
    uint8_t get_switches_value() const;

    /**
     * @brief Set the state of a switch.
     *
     * @param switch_index Index of the switch.
     * @param state New state of the switch.
     */
    void set_switch_state(uint8_t switch_index, bool state);
    
    /**
     * @brief Set the state of all switches based on a value.
     *
     * @param value Value representing the state of all switches.
     */
    void setState(uint8_t value);

private:
    b2BodyId bodyId;
    std::array<bool, num_of_switches> switch_states;
};

}  // namespace micras::proxy

#include "../src/dip_switch.cpp"  // NOLINT(bugprone-suspicious-include, misc-header-include-cycle)

#endif  // MICRAS_PROXY_DIP_SWITCH_HPP 
