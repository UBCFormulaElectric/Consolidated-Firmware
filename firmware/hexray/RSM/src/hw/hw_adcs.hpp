#pragma once
#include "hw_adc.hpp"

extern const hw::AdcChip<5> adc1;
extern const hw::Adc        lc3_out;
extern const hw::Adc        susp_travel_rl_3v3;
extern const hw::Adc        susp_travel_rr_3v3;
extern const hw::Adc        bps_3v3;
extern const hw::Adc        nBSPD_brake_pressed;

inline void adcchipsInit()
{
    LOG_IF_ERR(adc1.init());
}
