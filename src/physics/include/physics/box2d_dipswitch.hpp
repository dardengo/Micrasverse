#ifndef BOX2D_DIPSWITCH_HPP
#define BOX2D_DIPSWITCH_HPP

#include "physics/i_dipswitch.hpp"
#include <vector>

namespace micrasverse::physics {

/**
 * @brief Box2D implementation of a dip switch.
 */
class Box2DDipSwitch : public IDipSwitch {
public:
    /**
     * @brief Construct a new Box2DDipSwitch object.
     *
     * @param numSwitches The number of switches.
     */
    explicit Box2DDipSwitch(size_t numSwitches);

    /**
     * @brief Read the state of a switch by index.
     *
     * @param switchIndex The index of the switch.
     * @return True if the switch is on, false otherwise.
     */
    bool readSwitch(size_t switchIndex) const override;

    /**
     * @brief Set the state of a switch by index.
     *
     * @param switchIndex The index of the switch.
     * @param state The new state of the switch.
     */
    void setSwitch(size_t switchIndex, bool state) override;

    /**
     * @brief Toggle the state of a switch by index.
     *
     * @param switchIndex The index of the switch.
     */
    void toggleSwitch(size_t switchIndex) override;

private:
    std::vector<bool> switchStates;
};

} // namespace micrasverse::physics

#endif // BOX2D_DIPSWITCH_HPP 
