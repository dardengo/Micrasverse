#include "micras/proxy/buzzer.hpp"
#include "box2d/box2d.h"

namespace micras::proxy {

Buzzer::Buzzer(const Config& config) : 
    bodyId(config.bodyId), 
    volume(config.volume), 
    playing(false), 
    duration(0), 
    current_frequency(0) {
    tone_timer = std::make_unique<Stopwatch>(Stopwatch::Config{});
    wait_timer = std::make_unique<Stopwatch>(Stopwatch::Config{});
}

void Buzzer::play(uint32_t frequency, uint32_t duration_ms) {
    current_frequency = frequency;
    duration = duration_ms;
    playing = true;
    tone_timer->reset_ms();
}

void Buzzer::update() {
    if (playing && duration > 0) {
        if (tone_timer->elapsed_time_ms() >= duration) {
            stop();
        }
    }
}

void Buzzer::wait(uint32_t ms) {
    wait_timer->reset_ms();
    while (wait_timer->elapsed_time_ms() < ms) {
        update();
    }
}

void Buzzer::stop() {
    playing = false;
    current_frequency = 0;
}

bool Buzzer::is_playing() const {
    return playing;
}

}  // namespace micras::proxy 

