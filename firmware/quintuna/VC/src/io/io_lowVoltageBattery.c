#include "io_lowVoltageBattery.h"
#include "hw_gpios.h"

bool io_lowVoltageBattery_hasBoostControllerFault(void)
{
    // PGOOD pin on LTC3786 boost converter is pulled low when the output voltage is more than ±10 % away from the
    // regulated output voltage.
    return !hw_gpio_readPin(&pgood);
}

ChargingStatus io_lowVoltageBattery_chargingStatus(void)
{
    return hw_gpio_readPin(&bat_chrg_mode) ? CURRENT_REGULATING : VOLTAGE_REGULATING;
}

void io_lowVoltageBattery_chargingShutdown(bool state)
{
    hw_gpio_writePin(&bat_chrg_nshdn, !state);
}