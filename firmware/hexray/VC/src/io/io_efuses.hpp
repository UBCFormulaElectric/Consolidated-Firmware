#pragma once

#include "efuse/io_efuse.hpp"

constexpr int                                            NUM_EFUSE_CHANNELS = 9;
extern std::array<const io::Efuse *, NUM_EFUSE_CHANNELS> efuses;

extern const io::Efuse f_inv_efuse;
extern const io::Efuse rsm_efuse;
extern const io::Efuse bms_efuse;
extern const io::Efuse r_inv_efuse;
extern const io::Efuse dam_efuse;
extern const io::Efuse front_efuse;
extern const io::Efuse rl_pump_efuse;
extern const io::Efuse r_rad_fan_efuse;