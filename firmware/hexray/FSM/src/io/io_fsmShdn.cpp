#include "io_fsmShdn.hpp"
#include "hw_gpios.h"

namespace io::fsmShdn {
    bool COCKPIT_SHDN_OK_get(void)
    {
        return hw_gpio_readPin(&cockpit_shdn_3v3);
    }

    bool BOTS_SHDN_OK_get(void)
    {
        return hw_gpio_readPin(&bots_3v3);
    }

    bool FL_SHDN_OK_get(void)
    {
        return hw_gpio_readPin(&fl_int_3v3);
    }

    bool FR_SHDN_OK_get(void)
    {
        return hw_gpio_readPin(&fr_int_3v3);
    }
}