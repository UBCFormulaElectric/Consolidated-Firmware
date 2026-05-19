/*
INPUTS: NSHDN (from MCU, controlled based on status of bat alert pin...)
OUTPUTS: BAT_CHRG_MODE (to MCU)
Datasheet: https://www.analog.com/media/en/technical-documentation/data-sheets/MAX745.pdf
*/

#include "io_batteryCharging.hpp"
#include "hw_gpios.hpp"

namespace io::batteryCharging
{
void chargerEnable(void)
{
    bat_chrg_nSHDN.writePin(enable::HIGH);
}
void chargerDisable(void)
{
    bat_chrg_nSHDN.writePin(enable::LOW);
}
} // namespace io::batteryCharging