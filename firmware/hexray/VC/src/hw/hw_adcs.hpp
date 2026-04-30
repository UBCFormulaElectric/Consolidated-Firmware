#pragma once

#include "hw_adc.hpp"

extern const hw::adcchip<6> Adc_Chip1;
extern const hw::adcchip<5> Adc_Chip2;

extern const hw::adc adc_front;
extern const hw::adc adc_r_inv;
extern const hw::adc adc_bms;
extern const hw::adc adc_rl_pump;
extern const hw::adc adc_l_rad_fan;
extern const hw::adc adc_rr_pump;
extern const hw::adc adc_dam;
extern const hw::adc adc_r_rad_fan;
extern const hw::adc adc_misc_fuse;
extern const hw::adc adc_f_inv;
extern const hw::adc adc_rsm;

inline void adcChipsInit()
{
    LOG_IF_ERR(Adc_Chip1.init());
    LOG_IF_ERR(Adc_Chip2.init());
}