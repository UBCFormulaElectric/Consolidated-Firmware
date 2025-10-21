#include "io/brakeLight.h"

#include "hw/gpios.h"

void io_brakeLight_set(const bool val)
{
    hw_gpio_writePin(&brake_light_en_pin, val);
}
