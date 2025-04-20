#include "micras/proxy/box2d_sensor_adapter.hpp"
#include "physics/box2d_micrasbody.hpp"
#include "physics/box2d_distance_sensor.hpp"

namespace micras::proxy {

Box2DSensorAdapter::Box2DSensorAdapter(micrasverse::physics::Box2DMicrasBody* body, uint8_t sensorIndex)
    : body(body), sensorIndex(sensorIndex) {
}

float Box2DSensorAdapter::getReading() const {
    return body->getDistanceSensor(sensorIndex).getReading();
}

}  // namespace micras::proxy 
