#pragma once

#include "main.h"

namespace hw::adcs
{
void chipsInit();

extern const float *lc3_out;
extern const float *susp_travel_rl_3v3;
extern const float *susp_travel_rr_3v3;
extern const float *bps_3v3;
extern const float *nBSPD_brake_pressed;
} // namespace hw::adcs

extern "C"
{
    void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);
}