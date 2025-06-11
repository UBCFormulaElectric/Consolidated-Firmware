#include "io_brakeLight.h"

#include "hw_gpios.h"

void io_brakeLight_set(const bool val)
{
    hw_gpio_writePin(&brake_light_en_pin, val);
}
