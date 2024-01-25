#include "io_charger.h"
#include "app_globals.h"

static const Charger * charger;

void io_charger_init(const Charger * chgr)
{
    charger = chgr;
}

void io_charger_enable(bool enable)
{
    hw_gpio_writePin(&charger->enable_gpio, enable);
}

bool io_charger_isConnected(void)
{
    return hw_gpio_readPin(&charger->connected_gpio);
}

bool io_charger_hasFaulted(void)
{
    return hw_gpio_readPin(&charger->faulted_gpio);
}
