#include "gpio.h"
#include "ui/DimSwitches/DimSwitchEmitter.h"

#include <QThread>

static DimSwitchEmitter *dse = DimSwitchEmitter::getInstance();
// Functions handlers here correspond to the names given to the lines in altium.
void ROT_A(const gpio_edge edge)
{
    const Result<gpio_level, line_read_error> b_reading_res = read_gpio(gpio_input::GPIO8);
    if (b_reading_res.index() == 1)
    {
        qWarning("Got a GPIO Read Error when handling Quadrature: %d", get<line_read_error>(b_reading_res));
        return;
    }

    const gpio_level b_reading = get<gpio_level>(b_reading_res);
    // ReSharper disable once CppTooWideScope
    const bool right_rotating = ((b_reading == gpio_level::LOW) && edge == gpio_edge::RISING_EDGE) ||
                                ((b_reading == gpio_level::HIGH) && edge == gpio_edge::FALLING_EDGE);
    if (right_rotating)
        emit dse->rightRot();
    else
        emit dse->leftRot();
}
void ROT_B(const gpio_edge edge)
{
    const Result<gpio_level, line_read_error> a_reading_res = read_gpio(gpio_input::GPIO2);
    if (a_reading_res.index() == 1)
    {
        qWarning("Got a GPIO Read Error when handling Quadrature: %d", get<line_read_error>(a_reading_res));
        return;
    }

    const gpio_level a_reading = get<gpio_level>(a_reading_res);
    // ReSharper disable once CppTooWideScope
    const bool right_rotating = ((a_reading == gpio_level::HIGH) && edge == gpio_edge::RISING_EDGE) ||
                                ((a_reading == gpio_level::LOW) && edge == gpio_edge::FALLING_EDGE);
    if (right_rotating)
        emit dse->rightRot();
    else
        emit dse->leftRot();
}
void ROT_S(const gpio_edge edge)
{
    if (edge == gpio_edge::RISING_EDGE)
        emit dse->pushRot();
    else
        emit dse->pushRotReleased();
}
void OUT(const gpio_edge edge)
{
    if (edge == gpio_edge::RISING_EDGE)
        emit dse->outButtonPressed();
    else
        emit dse->outButtonReleased();
}
void ERR(const gpio_edge edge)
{
    if (edge == gpio_edge::RISING_EDGE)
        emit dse->errButtonPressed();
    else
        emit dse->errButtonReleased();
}
void STG(const gpio_edge edge)
{
    if (edge == gpio_edge::RISING_EDGE)
        emit dse->settingsButtonPressed();
    else
        emit dse->settingsButtonReleased();
}

void F1(const gpio_edge edge)
{
    if (edge == gpio_edge::RISING_EDGE)
        emit dse->f1ButtonPressed();
    else
        emit dse->f1ButtonReleased();
}
void F2(const gpio_edge edge)
{
    if (edge == gpio_edge::RISING_EDGE)
        emit dse->f2ButtonPressed();
    else
        emit dse->f2ButtonReleased();
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
            if (get<line_read_error>(l_event) == line_read_error::DEV_DUMMY_DATA)
                continue;
#ifdef USING_dimos_dev
            if (get<line_read_error>(l_event) == line_read_error::TIMEOUT)
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