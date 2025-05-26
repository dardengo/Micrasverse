#include "micras/proxy/stopwatch.hpp"
#include <thread>

namespace micras::proxy {

Stopwatch::Stopwatch() {
    this->reset_ms();
}

Stopwatch::Stopwatch(const Config& config) : looptime(config.looptime) {
    this->reset_ms();
}

void Stopwatch::reset_ms() {
    this->start_time = std::chrono::steady_clock::now();
}

void Stopwatch::reset_us() { }

uint32_t Stopwatch::elapsed_time_ms() const {
    auto current_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - this->start_time);
    return static_cast<uint32_t>(elapsed.count());
}

uint32_t Stopwatch::elapsed_time_us() const {
    return static_cast<uint32_t>(this->looptime * 1000000.0F);
}

void Stopwatch::sleep_ms(uint32_t time) {
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

void Stopwatch::sleep_us(uint32_t time) {
    std::this_thread::sleep_for(std::chrono::microseconds(time));
}

}  // namespace micras::proxy
