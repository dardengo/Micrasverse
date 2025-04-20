#include "micras/proxy/buzzer.hpp"

namespace micras::proxy {

Buzzer::Buzzer(const Config& config) : 
    micrasBody(config.micrasBody), 
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

void Buzzer::set_frequency(float frequency) {
    current_frequency = static_cast<uint32_t>(frequency);
    if (playing) {
        // If already playing, update the frequency without changing duration
        tone_timer->reset_ms();
    }
}

void Buzzer::set_duration(uint32_t duration_ms) {
    duration = duration_ms;
    if (playing) {
        // If already playing, update the duration without changing frequency
        tone_timer->reset_ms();
    }
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

