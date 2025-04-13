#ifndef MICRASVERSE_PHYSICS_I_DIPSWITCH_HPP
#define MICRASVERSE_PHYSICS_I_DIPSWITCH_HPP

#include <cstddef>

namespace micrasverse::physics {

class IDipSwitch {
public:
    virtual ~IDipSwitch() = default;

    // Read the state of a switch by index
    virtual bool readSwitch(size_t switchIndex) const = 0;

    // Set the state of a switch by index
    virtual void setSwitch(size_t switchIndex, bool state) = 0;

    // Toggle the state of a switch by index
    virtual void toggleSwitch(size_t switchIndex) = 0;
};

} // namespace micrasverse::physics

#endif // MICRASVERSE_PHYSICS_I_DIPSWITCH_HPP 
