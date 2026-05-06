#pragma once

#include "efuse/io_efuse.hpp"
#include "efuse/io_efuse_TI_TPS25.hpp"
#include "efuse/io_efuse_TI_TPS28.hpp"

#include <array>
#include <functional>

constexpr inline size_t                                                        NUM_EFUSE_CHANNELS = 10;
extern std::array<std::reference_wrapper<const io::Efuse>, NUM_EFUSE_CHANNELS> efuses;

extern const io::TI_TPS28_Efuse f_inv_efuse;
extern const io::TI_TPS28_Efuse rsm_efuse;
extern const io::TI_TPS28_Efuse bms_efuse;
extern const io::TI_TPS28_Efuse r_inv_efuse;
extern const io::TI_TPS28_Efuse dam_efuse;
extern const io::TI_TPS28_Efuse front_efuse;
extern const io::TI_TPS28_Efuse l_rad_fan_efuse;
extern const io::TI_TPS28_Efuse r_rad_fan_efuse;
extern const io::TI_TPS25_Efuse rl_pump_efuse;
extern const io::TI_TPS25_Efuse rr_pump_efuse;
