#ifndef DIPSWITCH_HPP
#define DIPSWITCH_HPP

#include "physics/i_dipswitch.hpp"
#include <vector>

namespace micrasverse::physics {

class DipSwitch : public IDipSwitch {
private:
    std::vector<bool> switches;                      // Stores the state of each switch

public:
    // Constructor: Initialize with the number of switches
    DipSwitch();
    DipSwitch(size_t numSwitches);

    // Implement IDipSwitch interface
    bool readSwitch(size_t switchIndex) const override;
    void setSwitch(size_t switchIndex, bool state) override;
    void toggleSwitch(size_t switchIndex) override;
};

} // namespace micrasverse::physics

#endif // DIPSWITCH_HPP
