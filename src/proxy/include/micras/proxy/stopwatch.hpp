#ifndef MICRAS_PROXY_STOPWATCH_HPP
#define MICRAS_PROXY_STOPWATCH_HPP

#include <cstdint>
#include <chrono>

namespace micras::proxy {

class Stopwatch {
public:
    struct Config {
        
    };

    Stopwatch();

    explicit Stopwatch(const Config& config);

    void reset_ms();

    void reset_us();

    uint32_t elapsed_time_ms() const;

    uint32_t elapsed_time_us() const;

    void sleep_ms(uint32_t time);

    void sleep_us(uint32_t time);

private:
    std::chrono::steady_clock::time_point start_time;
};

}  // namespace micras::proxy

#endif  // MICRAS_PROXY_STOPWATCH_HPP 
