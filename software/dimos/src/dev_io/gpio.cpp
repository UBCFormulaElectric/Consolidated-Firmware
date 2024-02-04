#include "gpio.h"

#include <iostream>
#include <array>
#include <qlogging.h>

#include "dev_io_utils.h"

using std::cout, std::endl;

std::array<bool, GPIO_COUNT> gpio_init()
{
    std::array<bool, GPIO_COUNT> out{};
    std::fill(out.begin(), out.end(), false);
    return out;
}

Result<gpio_edge, line_read_error> wait_for_line_event(gpio_input i)
{
    wait_delegate_thread();
    qInfo("Requested %s, responding with DEV_DUMMY_DATA", GPIO_inputs_info.at(i).enum_name.c_str());
    return DEV_DUMMY_DATA;
}