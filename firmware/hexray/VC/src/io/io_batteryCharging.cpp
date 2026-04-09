/*
INPUTS: NSHDN (from battery monitoring chip )
OUTPUTS: BAT_CHRG_MODE (to MCU)
        - high is voltage regulation (const voltage)
        - low is current regulation (const current)
        - idk seems pre unnecessary imma ignore for now and not do anything with it.
        - inshallah BQ IC can give us a better estimate of SOC w coulumb counting rather
        than a simple CC -> CV transition
*/

#include "io_batteryCharging.hpp"
#include "hw_gpios.hpp"

namespace io::batteryCharging
{

void chargerEnable(void)
{
    bat_chrg_nSHDN.writePin(HIGH);
}

void chargerDisable(void)
{
    bat_chrg_nSHDN.writePin(LOW);
}
} // namespace io::batteryCharging