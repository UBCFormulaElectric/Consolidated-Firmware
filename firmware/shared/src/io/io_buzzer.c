#include "io_buzzer.h"

void io_buzzer_enable(const Buzzer *buzzer, bool on)
{
    hw_gpio_writePin(&buzzer->gpio, on);
}
