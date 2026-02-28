#pragma once

#pragma once

#include "efuse/io_efuse.hpp"

namespace io
{
constexpr int NUM_EFUSE_CHANNELS = 9;

extern const Efuse *const efuse_channels[NUM_EFUSE_CHANNELS];

} // namespace io

extern const io::Efuse f_inv_efuse;
extern const io::Efuse rsm_efuse;
extern const io::Efuse bms_efuse;
extern const io::Efuse r_inv_efuse;
extern const io::Efuse dam_efuse;
extern const io::Efuse front_efuse;
extern const io::Efuse rl_pump_efuse;
extern const io::Efuse r_rad_fan_efuse;