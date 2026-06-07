#include "io_potentiometer.hpp"
#include "hw_i2cs.hpp"
#include "io_pumps.hpp"

const io::Potentiometer rr_pot{ rr_pump_i2c, io::POTENTIOMETER_WIPER::WIPER1 };
// MAKE SURE THAT YOU ARE GRABBING THE LOCK FOR THIS
const io::pump rr_pump{ rr_pot, false };
