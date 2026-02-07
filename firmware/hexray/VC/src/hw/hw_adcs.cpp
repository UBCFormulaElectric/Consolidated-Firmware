#include "hw_adcs.hpp"
#include "main.h"

namespace hw::adcs
{

#define NUM_ADC1_CHANNELS 6
#define NUM_ADC2_CHANNELS 5

AdcChip<NUM_ADC1_CHANNELS> Adc_Chip1(&hadc1, &htim3);
AdcChip<NUM_ADC1_CHANNELS> Adc_Chip2(&hadc2, &htim3);

void chipsInit()
{
    Adc_Chip1.init();
    Adc_Chip2.init();
}

const Adc adc_front{ Adc_Chip1.getChannel(0) };
const Adc adc_r_inv{ Adc_Chip1.getChannel(1) };
const Adc adc_bms{ Adc_Chip1.getChannel(2) };
const Adc adc_rl_pump{ Adc_Chip1.getChannel(3) };
const Adc adc_l_rad_fan{ Adc_Chip1.getChannel(4) };
const Adc adc_rr_pump{ Adc_Chip1.getChannel(5) };
const Adc adc_dam{ Adc_Chip2.getChannel(0) };
const Adc adc_r_rad_fan{ Adc_Chip2.getChannel(1) };
const Adc adc_misc_fuse{ Adc_Chip2.getChannel(2) };
const Adc adc_f_inv{ Adc_Chip2.getChannel(3) };
const Adc adc_rsm{ Adc_Chip2.getChannel(4) };
} // namespace hw::adcs

extern "C"
{
    void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
    {
        if (hadc == &hadc1)
        {
            hw::adcs::Adc_Chip1.update_callback();
        }
        else if (hadc == &hadc2)
        {
            hw::adcs::Adc_Chip2.update_callback();
        }
    }
}
