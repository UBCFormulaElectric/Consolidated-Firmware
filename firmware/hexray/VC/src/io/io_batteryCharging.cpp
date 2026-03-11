/*
DATASHEET: MAX745EAP+
INPUTS: BAT_CHRG_MODE (from MCU)
        - high is voltage regulation (const voltage) 
        - low is current regulation (const current)
        A battery that is fully discharged means current regulation first
        After the voltage has reached a certain threshold, it regulates voltage
OUTPUTS: 

*/

#include "io_batteryCharging.hpp"
#include "hw_gpios.hpp"

namespace io::batteryCharging {

    void chargerEnable(void) {
        bat_chrg_nSHDN.writePin(1);
    }

    void chargerDisable(void) {
        bat_chrg_nSHDN.writePin(0);
    }



}