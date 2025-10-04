#include "io_brakeLight.hpp"
#include "hw_gpios.hpp" //Not created yet

void io_brakeLight_set(const bool val)
{
    //tells the pin which controls light to adpot value given
    hw_gpio_writePin(&brake_light_en_pin, val);
}


