#include "io_fsmShdn.h"
#include "hw_gpios.h"

bool io_fsmShdn_COCKPIT_SHDN_OK_get(void)
{
    return hw_gpio_readPin(&cockpit_shdn_3v3);
}

bool io_fsmShdn_BOTS_SHDN_OK_get(void)
{
    return hw_gpio_readPin(&bots_3v3);
}

bool io_fsmShdn_FL_SHDN_OK_get(void)
{
    return hw_gpio_readPin(&fl_int_3v3);
}

bool io_fsmShdn_FR_SHDN_OK_get(void)
{
    return hw_gpio_readPin(&fr_int_3v3);
}
