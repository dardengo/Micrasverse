#ifndef DIPSWITCH_HPP
#define DIPSWITCH_HPP

#include <vector>
#include <string>
#include <unordered_map>

namespace micrasverse::physics {

class DipSwitch {
private:
    std::vector<bool> switches;                      // Stores the state of each switch

public:
    // Constructor: Initialize with the number of switches
    DipSwitch();
    DipSwitch(size_t numSwitches);

    // Read the state of a switch by index
    bool readSwitch(size_t switchIndex) const;

    // Set the state of a switch by index
    void setSwitch(size_t switchIndex, bool state);

    // Toggle the state of a switch by index
    void toggleSwitch(size_t switchIndex);
};

} // namespace micrasverse::physics

#endif // DIPSWITCH_HPP
