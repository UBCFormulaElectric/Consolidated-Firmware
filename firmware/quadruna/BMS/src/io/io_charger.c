#include "io_charger.h"
#include "app_globals.h"

static const Charger *charger;

void io_charger_init(const Charger *chgr)
{
    // charger = chgr;
    UNUSED(chgr);
}

void io_charger_enable(bool enable)
{
    // hw_gpio_writePin(&charger->enable_gpio, enable);
    UNUSED(enable);
}

bool io_charger_isConnected(void)
{
    // return hw_gpio_readPin(&charger->connected_gpio);
    return false;
}

bool io_charger_hasFaulted(void)
{
    // return hw_gpio_readPin(&charger->faulted_gpio);
    return true;
}
