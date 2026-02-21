#include "hw_adcs.hpp"
#include "main.h"

namespace hw::adcs
{
constexpr int             NUM_ADC_CHANNELS = 5;
AdcChip<NUM_ADC_CHANNELS> Adc_Chip(&hadc1, &htim3);

void chipsInit()
{
    Adc_Chip.init();
}

Adc lc3_out{ Adc_Chip.getChannel(0) };
Adc susp_travel_rl_3v3{ Adc_Chip.getChannel(1) };
Adc susp_travel_rr_3v3{ Adc_Chip.getChannel(2) };
Adc bps_3v3{ Adc_Chip.getChannel(3) };
Adc nBSPD_brake_pressed{ Adc_Chip.getChannel(4) };

} // namespace hw::adcs

extern "C"
{
    void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
    {
        hw::adcs::Adc_Chip.update_callback();
    }
}