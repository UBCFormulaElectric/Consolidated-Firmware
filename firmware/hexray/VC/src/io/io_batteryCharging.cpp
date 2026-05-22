/*
INPUTS: NSHDN (from MCU, controlled based on status of bat alert pin...)
OUTPUTS: BAT_CHRG_MODE (to MCU)
Datasheet: https://www.analog.com/media/en/technical-documentation/data-sheets/MAX745.pdf
*/

#include "io_batteryCharging.hpp"
#include "hw_gpios.hpp"

namespace io::batteryCharging
{
void charger_enable(void)
{
    bat_chrg_nSHDN.writePin(enable::HIGH);
}
void charger_disable(void)
{
    bat_chrg_nSHDN.writePin(enable::LOW);
}
bool check_status(void)
{
    return bat_chrg_nSHDN.readPin(); 
}
} // namespace io::batteryCharging