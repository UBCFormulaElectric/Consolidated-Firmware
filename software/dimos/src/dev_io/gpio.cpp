#include "gpio.h"

// PUBLIC FUNCTIONS

std::array<bool, GPIO_COUNT> gpio_init() {
    return {false};
}

Result<gpio_edge, line_read_error> wait_for_line_event(gpio_input i) {
    return RISING_EDGE;
}