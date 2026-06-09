#include "io_pumps.hpp"
#include "hw_i2cs.hpp"

const io::Potentiometer rl_pump_pot{ rl_pump_i2c, io::POTENTIOMETER_WIPER::WIPER0 };
constexpr io::pump      rl_pump{ rl_pump_pot, false };
