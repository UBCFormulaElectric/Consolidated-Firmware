#pragma once
#include "hw_adc.hpp"

extern const hw::adcchip<5> adc1;
extern const hw::adc        lc3_out;
extern const hw::adc        susp_travel_rl_3v3;
extern const hw::adc        susp_travel_rr_3v3;
extern const hw::adc        bps_3v3;
extern const hw::adc        nBSPD_brake_pressed;

inline void adcchipsInit()
{
    LOG_IF_ERR(adc1.init());
}
