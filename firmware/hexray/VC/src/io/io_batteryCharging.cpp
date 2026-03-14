/*
DATASHEET: MAX745EAP+
INPUTS: NSHDN (from battery monitoring chip )
OUTPUTS: BAT_CHRG_MODE (to MCU)
        - high is voltage regulation (const voltage) 
        - low is current regulation (const current)
        - idk seems pre unnecessary 
*/

#include "io_batteryCharging.hpp"
#include "hw_gpios.hpp"

constexpr float SHDNRise = 2.34;
constexpr float SHDNFall = 2.19;

namespace io::batteryCharging {

    void chargerEnable(void) {
        bat_chrg_nSHDN.writePin(1);
    }

    void chargerDisable(void) {
        bat_chrg_nSHDN.writePin(0);
    }



}