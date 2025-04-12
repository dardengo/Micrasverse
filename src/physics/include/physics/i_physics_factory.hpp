#ifndef I_PHYSICS_FACTORY_HPP
#define I_PHYSICS_FACTORY_HPP

#include "physics/i_argb.hpp"
#include "physics/i_dipswitch.hpp"
#include "physics/i_distance_sensor.hpp"
#include "physics/i_motor.hpp"
#include "core/types.hpp"

#include <memory>
#include <string>

namespace micrasverse::physics {

class IPhysicsFactory {
public:
    virtual ~IPhysicsFactory() = default;

    // Create components
    virtual std::unique_ptr<IARGB> createARGB(
        const core::Vec2& localPosition, 
        const core::Vec2& size, 
        const core::Color& color) = 0;

    virtual std::unique_ptr<IDipSwitch> createDipSwitch(size_t numSwitches) = 0;

    virtual std::unique_ptr<IDistanceSensor> createDistanceSensor(
        const core::Vec2& localPosition, 
        float angle) = 0;

    virtual std::unique_ptr<IMotor> createMotor(
        const core::Vec2& localPosition, 
        bool leftWheel, 
        float angle,
        float resistance, 
        float ke, 
        float kt, 
        float maxVoltage) = 0;
};

} // namespace micrasverse::physics

#endif // I_PHYSICS_FACTORY_HPP 
