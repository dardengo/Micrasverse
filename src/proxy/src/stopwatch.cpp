#include "micras/proxy/stopwatch.hpp"
#include "box2d/box2d.h"

namespace micras::proxy {

Stopwatch::Stopwatch() {
    this->reset_ms();
}

Stopwatch::Stopwatch(const Config& config, const b2BodyId bodyId) :
    bodyId{bodyId} {
    this->reset_ms();
}

void Stopwatch::reset_ms() {
    b2WorldId world = b2Body_GetWorld(bodyId);
    b2Profile profile = b2World_GetProfile(world);
    this->start_time = profile.step;
}

void Stopwatch::reset_us() {
    b2WorldId world = b2Body_GetWorld(bodyId);
    b2Profile profile = b2World_GetProfile(world);
    this->start_time = profile.step;
}

uint32_t Stopwatch::elapsed_time_ms() const {
    b2WorldId world = b2Body_GetWorld(bodyId);
    b2Profile profile = b2World_GetProfile(world);
    float current_time = profile.step;
    return static_cast<uint32_t>((current_time - this->start_time) * 1000.0f);
}

uint32_t Stopwatch::elapsed_time_us() const {
    b2WorldId world = b2Body_GetWorld(bodyId);
    b2Profile profile = b2World_GetProfile(world);
    float current_time = profile.step;
    return static_cast<uint32_t>((current_time - this->start_time) * 1000000.0f);
}

void Stopwatch::sleep_ms(uint32_t time) {
    b2WorldId world = b2Body_GetWorld(bodyId);
    b2Profile profile = b2World_GetProfile(world);
    float start_sleep = profile.step;
    float sleep_end = start_sleep + (time / 1000.0f);
    
    while (b2World_GetProfile(world).step < sleep_end) {
        // Busy wait
    }
}

void Stopwatch::sleep_us(uint32_t time) {
    b2WorldId world = b2Body_GetWorld(bodyId);
    b2Profile profile = b2World_GetProfile(world);
    float start_sleep = profile.step;
    float sleep_end = start_sleep + (time / 1000000.0f);
    
    while (b2World_GetProfile(world).step < sleep_end) {
        // Busy wait
    }
}

}  // namespace micras::proxy 
