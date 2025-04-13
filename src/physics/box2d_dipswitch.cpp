#include "physics/box2d_dipswitch.hpp"
#include <stdexcept>

namespace micrasverse::physics {

Box2DDipSwitch::Box2DDipSwitch(size_t numSwitches) {
    // Initialize all switches to off (false)
    switchStates.resize(numSwitches, false);
}

bool Box2DDipSwitch::readSwitch(size_t switchIndex) const {
    if (switchIndex >= switchStates.size()) {
        throw std::out_of_range("Switch index out of range");
    }
    return switchStates[switchIndex];
}

void Box2DDipSwitch::setSwitch(size_t switchIndex, bool state) {
    if (switchIndex >= switchStates.size()) {
        throw std::out_of_range("Switch index out of range");
    }
    switchStates[switchIndex] = state;
}

void Box2DDipSwitch::toggleSwitch(size_t switchIndex) {
    if (switchIndex >= switchStates.size()) {
        throw std::out_of_range("Switch index out of range");
    }
    switchStates[switchIndex] = !switchStates[switchIndex];
}

} // namespace micrasverse::physics 
