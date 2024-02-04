#include "gpio.h"

#include <QThread>
#include <iostream>

void Rot_PushButton(const gpio_edge edge)
{
    Q_UNUSED(edge);
}
void Rot_Quadrature(const gpio_edge edge)
{
    Q_UNUSED(edge);
}
void Sw_Up(const gpio_edge edge)
{
    Q_UNUSED(edge);
}
void Sw_Down(const gpio_edge edge)
{
    Q_UNUSED(edge);
}
void Sw_Left(const gpio_edge edge)
{
    Q_UNUSED(edge);
}
void Sw_Right(const gpio_edge edge)
{
    Q_UNUSED(edge);
}
void Sw_1(const gpio_edge edge)
{
    Q_UNUSED(edge);
}

// gpio handlers
constexpr void (*gpio_handler_funcs[])(gpio_edge) = { &Rot_PushButton, &Rot_Quadrature, &Rot_Quadrature, &Sw_Up,
                                                      &Sw_Right,       &Sw_Left,        &Sw_Down,        &Sw_1 };

namespace gpio_handlers
{
void gpio_monitor(const gpio_input i)
{
    qInfo("%s thread started", GPIO_inputs_info.at(i).enum_name.c_str());
    while (!QThread::currentThread()->isInterruptionRequested())
    {
        const auto l_event = wait_for_line_event(i);
        if (l_event.index() == 1)
        {
            if (get<line_read_error>(l_event) == DEV_DUMMY_DATA)
                continue;
            qWarning("GPIO READ ERROR");
            continue;
        }
        gpio_handler_funcs[i](std::get<gpio_edge>(l_event));
    }
    qInfo("exiting %s thread now", GPIO_inputs_info.at(i).enum_name.c_str());
}
} // namespace gpio_handlers