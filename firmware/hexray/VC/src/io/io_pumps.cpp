#include "io_potentiometer.hpp"
#include "hw_i2cs.hpp"
#include "io_pumps.hpp"

const io::Potentiometer rr_pot{ rr_pump_i2c, io::POTENTIOMETER_WIPER::WIPER1 };
const io::pump          rr_pump{ rr_pot, false };
