#ifndef MICRAS_PROXY_BOX2D_SENSOR_ADAPTER_HPP
#define MICRAS_PROXY_BOX2D_SENSOR_ADAPTER_HPP

#include <cstdint>

// Forward declaration
namespace micrasverse::physics {
class Box2DMicrasBody;
}  // namespace micrasverse::physics

namespace micras::proxy {

// Adapter that wraps a Box2DMicrasBody distance sensor
class Box2DSensorAdapter {
public:
    Box2DSensorAdapter(micrasverse::physics::Box2DMicrasBody* body, uint8_t sensorIndex);

    // Implementation of the interface method
    float getReading() const;

private:
    micrasverse::physics::Box2DMicrasBody* body;
    uint8_t                                sensorIndex;
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_BOX2D_SENSOR_ADAPTER_HPP
