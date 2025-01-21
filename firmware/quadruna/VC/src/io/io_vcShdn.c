#include "io_vcShdn.h"
#include "hw_gpios.h"

bool io_vcShdn_TsmsFault_get(void)
{
    return hw_gpio_readPin(&tsms_shdn_sns);
}

bool io_vcShdn_LEStopFault_get(void)
{
    return hw_gpio_readPin(&l_shdn_sns);
}

bool io_vcShdn_REStopFault_get(void)
{
    return hw_gpio_readPin(&r_shdn_sns);
}

bool io_vcShdn_SplitterBoxInterlockFault_get(void)
{
    return hw_gpio_readPin(&sb_ilck_shdn_sns);
}
