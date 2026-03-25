#pragma once

#include "io_pump.hpp"
#include "io_potentiometer.hpp"
#include "hw_i2cs.hpp"
#include "io_efuses.hpp"

extern const io::Potentiometer pot{ hw::i2c::pwr_pump, io::POTENTIOMETER_WIPER::WIPER0 };
extern const io::Potentiometer rPump{ hw::i2c::pwr_pump, io::POTENTIOMETER_WIPER::WIPER0 }; // TEMOP!!!

extern const io::Pump rr_pump{ &pot, false, &rr_pump_efuse };
extern const io::Pump rl_pump{ &rPump, false, &rl_pump_efuse };