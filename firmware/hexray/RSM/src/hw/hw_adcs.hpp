#pragma once
namespace hw::adcs
{
void chipsInit();

extern Adc lc3_out;
extern Adc susp_travel_rl_3v3;
extern Adc susp_travel_rr_3v3;
extern Adc bps_3v3;
extern Adc nBSPD_brake_pressed;
} // namespace hw::adcs