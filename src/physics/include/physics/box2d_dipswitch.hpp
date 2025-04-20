#ifndef BOX2D_DIPSWITCH_HPP
#define BOX2D_DIPSWITCH_HPP

#include "physics/i_dipswitch.hpp"
#include <vector>

namespace micrasverse::physics {

class Box2DDipSwitch : public IDipSwitch {
public:
    explicit Box2DDipSwitch(size_t numSwitches);

    bool readSwitch(size_t switchIndex) const override;

    void setSwitch(size_t switchIndex, bool state) override;

    void toggleSwitch(size_t switchIndex) override;

private:
    std::vector<bool> switchStates;
};

} // namespace micrasverse::physics

#endif // BOX2D_DIPSWITCH_HPP 
