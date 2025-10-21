#include "io/buzzer.h"
#include "hw/gpios.h"

void io_enable_buzzer(void)
{
    hw_gpio_writePin(&buzzer_pwr_en_pin, true);
}

void io_disable_buzzer(void)
{
    hw_gpio_writePin(&buzzer_pwr_en_pin, false);
}