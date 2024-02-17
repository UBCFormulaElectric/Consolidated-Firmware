#include "gpio.h"

#include <QThread>
#include <iostream>
#include <QtLogging>

// Functions handlers here correspond to the names given to the lines in altium.
void ROT_A(const gpio_edge edge)
{
    Q_UNUSED(edge);
}
void ROT_B(const gpio_edge edge)
{
    Q_UNUSED(edge);
}
void ROT_S(const gpio_edge edge)
{
    if (edge == RISING_EDGE)
    {
        qInfo("Rotary Push Button Pressed");
    }
    else
    {
        qInfo("Rotary Push Button Released");
    }
}
void OUT(const gpio_edge edge)
{
    Q_UNUSED(edge);
}
void ERR(const gpio_edge edge)
{
    Q_UNUSED(edge);
}
void STG(const gpio_edge edge)
{
    Q_UNUSED(edge);
}

void F1(const gpio_edge edge)
{
    Q_UNUSED(edge);
}
void F2(const gpio_edge edge)
{
    Q_UNUSED(edge);
}

// gpio handlers
const std::map<gpio_input, void (*)(gpio_edge)> gpio_handler_funcs{
    { gpio_input::GPIO1, &F1 },    { gpio_input::GPIO2, &ROT_A }, { gpio_input::GPIO3, &OUT },
    { gpio_input::GPIO4, &F2 },    { gpio_input::GPIO5, &STG },   { gpio_input::GPIO6, &ERR },
    { gpio_input::GPIO7, &ROT_S }, { gpio_input::GPIO8, &ROT_B },
};

namespace gpio_handlers
{
void gpio_monitor(const gpio_input i)
{
    qInfo("%s thread started", GPIO_inputs_info.at(i).enum_name.c_str());
    while (!QThread::currentThread()->isInterruptionRequested())
    {
        const Result<gpio_edge, line_read_error> l_event = wait_for_line_event(i);
        if (l_event.index() == 1)
        {
            if (get<line_read_error>(l_event) == DEV_DUMMY_DATA)
                continue;
#ifdef USING_dimos_dev
            if (get<line_read_error>(l_event) == TIMEOUT)
                continue;
#endif
            qWarning("GPIO READ ERROR: %d", get<line_read_error>(l_event));
            continue;
        }
        gpio_handler_funcs.at(i)(std::get<gpio_edge>(l_event));
    }
    qInfo("KILL GPIO thread: %s", GPIO_inputs_info.at(i).enum_name.c_str());
}
} // namespace gpio_handlers