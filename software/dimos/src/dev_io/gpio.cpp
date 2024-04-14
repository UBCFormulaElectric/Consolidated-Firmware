#include "gpio.h"

#include <iostream>
#include <qlogging.h>

#include "dev_io_utils.h"

using std::cout, std::endl;

std::map<gpio_input, bool> gpio_init()
{
    std::map<gpio_input, bool> out{};
    for (const gpio_input i : gpio_inputs)
        out[i] = false;
    return out;
}

Result<gpio_edge, line_read_error> wait_for_line_event(gpio_input i)
{
    if (const WaitDelegateResult res = wait_delegate_thread(); res == WaitDelegateResult::INTERRUPTED)
        return line_read_error::TIMEOUT;
    qInfo("Requested %s, responding with DEV_DUMMY_DATA", gpio_inputs_metadata.at(i).enum_name.c_str());
    return line_read_error::DEV_DUMMY_DATA;
}

Result<gpio_level, line_read_error> read_gpio(gpio_input i)
{
    Q_UNUSED(i);
    return line_read_error::DEV_DUMMY_DATA;
}