#ifndef BOX2D_DIPSWITCH_HPP
#define BOX2D_DIPSWITCH_HPP

#include <vector>

namespace micrasverse::physics {

class Box2DDipSwitch {
public:
    explicit Box2DDipSwitch(size_t numSwitches);

    bool readSwitch(size_t switchIndex) const;

    void setSwitch(size_t switchIndex, bool state);

    void toggleSwitch(size_t switchIndex);

private:
    std::vector<bool> switchStates;
};

}  // namespace micrasverse::physics

#endif  // BOX2D_DIPSWITCH_HPP
