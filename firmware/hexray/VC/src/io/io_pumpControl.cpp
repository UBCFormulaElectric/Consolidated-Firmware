#include "io_potentiometer.hpp"
#include "hw_i2cs.hpp"
#include "io_efuses.hpp"
#include "io_pumpControl.hpp"

io::Potentiometer pot{ hw::i2c::pwr_pump, io::POTENTIOMETER_WIPER::WIPER0 };
io::Potentiometer rPump{ hw::i2c::pwr_pump, io::POTENTIOMETER_WIPER::WIPER0 }; // TEMOP!!!

io::Pump rr_pump{ pot, false, rr_pump_efuse };
io::Pump rl_pump{ rPump, false, rl_pump_efuse };