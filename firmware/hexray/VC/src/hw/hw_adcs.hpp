#pragma once

#include "hw_adc.hpp"

namespace hw::adcs
{
void                chipsInit();
extern const float *front_sns;
extern const float *r_inv_sns;
extern const float *bms_sns;
extern const float *rl_pump_i_sns;
extern const float *l_rad_fan_i_sns;
extern const float *rr_pump_i_sns;
extern const float *dam_sns;
extern const float *r_rad_fan_i_sns;
extern const float *misc_fuse_sns;
extern const float *f_inv_sns;
extern const float *rsm_sns;
} // namespace hw::adcs

extern "C"
{
}
