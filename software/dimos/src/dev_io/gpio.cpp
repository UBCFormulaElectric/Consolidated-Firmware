#include "gpio.h"
#include <iostream>

// PUBLIC FUNCTIONS

std::array<bool, GPIO_COUNT> gpio_init()
{
    std::array<bool, GPIO_COUNT> out{};
    std::fill(out.begin(), out.end(), true); // tell the caller that all GPIOs are errored
    return out;
}

Result<gpio_edge, line_read_error> wait_for_line_event(gpio_input i)
{
    std::cout << "Requested GPIO Input DEV" << std::endl;
    return RISING_EDGE; // shouldn't ever be read
}