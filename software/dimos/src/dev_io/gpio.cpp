#include "gpio.h"

#include <iostream>
#include <qlogging.h>

#include "dev_io_utils.h"

using std::cout, std::endl;

std::map<gpio_input, bool> gpio_init()
{
    std::map<gpio_input, bool> out{};
    for (const gpio_input i : gpio_inputs)
        out[i] = true;
    return out;
}

Result<gpio_edge, line_read_error> wait_for_line_event(gpio_input i)
{
    if (const WaitDelegateResult res = wait_delegate_thread(); res == WaitDelegateResult::INTERRUPTED)
        return TIMEOUT;
    qInfo("Requested %s, responding with DEV_DUMMY_DATA", GPIO_inputs_info.at(i).enum_name.c_str());
    return DEV_DUMMY_DATA;
}