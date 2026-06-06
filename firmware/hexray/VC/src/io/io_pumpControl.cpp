#include "io_potentiometer.hpp"
#include "hw_i2cs.hpp"
#include "io_efuses.hpp"
#include "io_pumpControl.hpp"

const io::Potentiometer rr_pot{ pump, io::POTENTIOMETER_WIPER::WIPER1 };
const io::pump          rr_pump{ rr_pot, false, rr_pump_efuse };
