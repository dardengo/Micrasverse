#ifndef MICRASVERSE_PHYSICS_I_PHYSICS_FACTORY_HPP
#define MICRASVERSE_PHYSICS_I_PHYSICS_FACTORY_HPP

#include "physics/i_argb.hpp"
#include "physics/i_dipswitch.hpp"
#include "physics/i_distance_sensor.hpp"
#include "physics/i_motor.hpp"
#include <memory>
#include <string>
#include <vector>

#include "micrasverse_core/types.hpp"

namespace micrasverse::physics {

class IPhysicsFactory {
public:
    virtual ~IPhysicsFactory() = default;

    // Create components
    virtual std::unique_ptr<IArgb> createARGB(
        const types::Vec2& localPosition, 
        const types::Vec2& size, 
        const types::Color& color) = 0;

    virtual std::unique_ptr<IDipSwitch> createDipSwitch(size_t numSwitches) = 0;

    virtual std::unique_ptr<IDistanceSensor> createDistanceSensor(
        const types::Vec2& localPosition, 
        float angle) = 0;

    virtual std::unique_ptr<IMotor> createMotor(
        const types::Vec2& localPosition, 
        bool leftWheel, 
        float angle,
        float resistance, 
        float ke, 
        float kt, 
        float maxVoltage) = 0;
};

} // namespace micrasverse::physics

#endif // MICRASVERSE_PHYSICS_I_PHYSICS_FACTORY_HPP 
