#pragma once

#include "hw_adc.hpp"

namespace hw::adcs
{
extern const AdcChip<6> Adc_Chip1;
extern const AdcChip<5> Adc_Chip2;

extern const Adc adc_front;
extern const Adc adc_r_inv;
extern const Adc adc_bms;
extern const Adc adc_rl_pump;
extern const Adc adc_l_rad_fan;
extern const Adc adc_rr_pump;
extern const Adc adc_dam;
extern const Adc adc_r_rad_fan;
extern const Adc adc_misc_fuse;
extern const Adc adc_f_inv;
extern const Adc adc_rsm;
} // namespace hw::adcs
