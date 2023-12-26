#include "gpio.h"
#include <QtGlobal>

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
constexpr void (*gpio_handler_funcs[])(gpio_edge) = { &Rot_PushButton, &Rot_Quadrature, &Rot_Quadrature, &Sw_Up, &Sw_Right, &Sw_Left, &Sw_Down, &Sw_1 };

namespace gpio_handlers
{
[[noreturn]] void gpio_monitor(const gpio_input i)
{
    forever
    {
        const auto l_event = wait_for_line_event(i);
        if (l_event.index() == 1)
        {
            continue;
        }
        gpio_handler_funcs[i](std::get<gpio_edge>(l_event));
    }
}
} // namespace gpio_handlers