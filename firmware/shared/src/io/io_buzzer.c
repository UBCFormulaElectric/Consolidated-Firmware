#include "io_buzzer.h"

static const Buzzer *buzzer;

void io_buzzer_enable(bool on)
{
    hw_gpio_writePin(&buzzer->gpio, on);
}

void io_buzzer_init(const Buzzer *buzzer_in)
{
    buzzer = buzzer_in;
    hw_gpio_writePin(&buzzer->gpio, false);
}
