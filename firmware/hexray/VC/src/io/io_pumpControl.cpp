#include "io_potentiometer.hpp"
#include "hw_i2cs.hpp"
#include "io_efuses.hpp"
#include "io_pumpControl.hpp"

const io::Potentiometer rr_pot{ pwr_pump, io::POTENTIOMETER_WIPER::WIPER0 };
const io::Potentiometer rl_pot{ pwr_pump, io::POTENTIOMETER_WIPER::WIPER1 };
const io::Pump          rr_pump{ rr_pot, false, rr_pump_efuse };
const io::Pump          rl_pump{
    rl_pot, false, rl_pump_efuse
}; // a bit confused regarding whetehr we need this one from altium seems liek we done?
