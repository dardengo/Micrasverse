#pragma once

#include <cstdint>
#include "physics/box2d_micrasbody.hpp"

namespace micras::proxy {

class Button {
public:
    enum class Status {
        NO_PRESS,
        SHORT_PRESS,
        LONG_PRESS,
        EXTRA_LONG_PRESS
    };

    enum class PullType {
        PULL_UP,
        PULL_DOWN,
        NO_PULL
    };

    struct Config {
        micrasverse::physics::Box2DMicrasBody* micrasBody = nullptr;
        bool initial_state = false;
        PullType pull_type = PullType::PULL_UP;
    };

    explicit Button(const Config& config);
    
    void update();

    bool is_pressed() const;
    Status get_status() const;
    void set_state(bool state);
    void set_status(Status status);
    PullType get_pull_type() const;
    void set_pull_type(PullType pull_type);

private:
    micrasverse::physics::Box2DMicrasBody* micrasBody;
    bool current_state;
    bool previous_state;
    Status current_status;
    PullType pull_type;
    
    bool get_logical_state() const;
};

} // namespace micras::proxy 
