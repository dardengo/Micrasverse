#include "micras/proxy/imu.hpp"
#include "constants.hpp"
#include <random>

namespace micras::proxy {

Imu::Imu(const Config& config) :
    micrasBody{config.micrasBody},
    gyroscope_noise{config.gyroscope_noise},
    accelerometer_noise{config.accelerometer_noise} {
    bodyId = micrasBody->getBodyId();
}

bool Imu::check_whoami() {
    // In simulation, we always return true since we're simulating a working IMU
    return true;
}

void Imu::update() {
    // Get the body's angular velocity and linear velocity from Box2D
    float angularVelocity = b2Body_GetAngularVelocity(bodyId);
    b2Vec2 current_linear_velocity = b2Body_GetLinearVelocity(bodyId);
    b2Vec2 lin_acc = (current_linear_velocity - previous_linear_velocity) * ( 1 / (loop_time_us / 1000000.0f) ) ;
    previous_linear_velocity = current_linear_velocity;

    // Add noise to simulate real IMU behavior
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::normal_distribution<float> gyro_noise(0.0f, gyroscope_noise);
    static std::normal_distribution<float> accel_noise(0.0f, accelerometer_noise);

    // Convert Box2D units to IMU units and add noise
    // Box2D uses radians/s for angular velocity and m/s for velocity
    angular_velocity[0] = 0.0f; // No X rotation in 2D
    angular_velocity[1] = 0.0f; // No Y rotation in 2D
    angular_velocity[2] = angularVelocity + gyro_noise(gen); // Z rotation (yaw)

    // For acceleration, we'll use velocity changes over time
    linear_acceleration[0] = lin_acc.x + accel_noise(gen);
    linear_acceleration[1] = lin_acc.y + accel_noise(gen);
    linear_acceleration[2] = 9.81f + accel_noise(gen); // Gravity in Z direction
}

float Imu::get_angular_velocity(Axis axis) const {
    switch (axis) {
        case Axis::X:
            return angular_velocity[0];
        case Axis::Y:
            return angular_velocity[1];
        case Axis::Z:
            return angular_velocity[2];
        default:
            return 0.0f;
    }
}

float Imu::get_linear_acceleration(Axis axis) const {
    switch (axis) {
        case Axis::X:
            return linear_acceleration[0];
        case Axis::Y:
            return linear_acceleration[1];
        case Axis::Z:
            return linear_acceleration[2];
        default:
            return 0.0f;
    }
}

void Imu::calibrate() {
    calibrated = true;
}

bool Imu::was_initialized() const {
    return this->initialized;
}

}  // namespace micras::proxy 
