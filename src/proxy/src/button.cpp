#include "micras/proxy/button.hpp"

namespace micras::proxy {

Button::Button(const Config& config) :
    micrasBody(config.micrasBody),
    current_state(config.initial_state),
    previous_state(config.initial_state),
    current_status(Status::NO_PRESS),
    pull_type(config.pull_type) { }

bool Button::is_pressed() const {
    return get_logical_state();
}

void Button::update() { }

Button::Status Button::get_status() const {
    return current_status;
}

void Button::set_state(bool state) {
    previous_state = current_state;
    current_state = state;

    bool logical_state = get_logical_state();

    if (!logical_state) {
        current_status = Status::NO_PRESS;
    } else if (current_status == Status::NO_PRESS) {
        current_status = Status::SHORT_PRESS;
    }
}

void Button::set_status(Status status) {
    current_status = status;

    if (status == Status::NO_PRESS) {
        current_state = (pull_type == PullType::PULL_UP);
    } else {
        current_state = (pull_type == PullType::PULL_DOWN);
    }
}

Button::PullType Button::get_pull_type() const {
    return pull_type;
}

void Button::set_pull_type(PullType pull_type) {
    this->pull_type = pull_type;
}

bool Button::get_logical_state() const {
    return (pull_type == PullType::PULL_UP) ? !current_state : current_state;
}

}  // namespace micras::proxy
