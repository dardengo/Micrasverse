#include "physics/dipswitch.hpp"

#include <stdexcept>
#include <iostream>

namespace micrasverse::physics {

// Constructor: Initialize with the number of switches
DipSwitch::DipSwitch() : switches(4, false) {}
DipSwitch::DipSwitch(size_t numSwitches) : switches(numSwitches, false) {}

// Read the state of a switch by index
bool DipSwitch::readSwitch(size_t switchIndex) const {
    return switches[switchIndex];
}

// Set the state of a switch by index
void DipSwitch::setSwitch(size_t switchIndex, bool state) {
    switches[switchIndex] = state;
}

// Toggle the state of a switch by index
void DipSwitch::toggleSwitch(size_t switchIndex) {
    switches[switchIndex] = !switches[switchIndex];
}

} // namespace micrasverse::physics
