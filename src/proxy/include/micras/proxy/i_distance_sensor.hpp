#ifndef MICRAS_PROXY_I_DISTANCE_SENSOR_HPP
#define MICRAS_PROXY_I_DISTANCE_SENSOR_HPP

namespace micras::proxy {

// Interface for any distance sensor
class IDistanceSensor {
public:
    virtual ~IDistanceSensor() = default;
    
    // Get raw sensor reading
    virtual float getReading() const = 0;
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_I_DISTANCE_SENSOR_HPP 
