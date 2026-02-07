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

const float *front_sns       = Adc_Chip1.getChannel(0);
const float *r_inv_sns       = Adc_Chip1.getChannel(1);
const float *bms_sns         = Adc_Chip1.getChannel(2);
const float *rl_pump_i_sns   = Adc_Chip1.getChannel(3);
const float *l_rad_fan_i_sns = Adc_Chip1.getChannel(4);
const float *rr_pump_i_sns   = Adc_Chip1.getChannel(5);
const float *dam_sns         = Adc_Chip2.getChannel(0);
const float *r_rad_fan_i_sns = Adc_Chip2.getChannel(1);
const float *misc_fuse_sns   = Adc_Chip2.getChannel(2);
const float *f_inv_sns       = Adc_Chip2.getChannel(3);
const float *rsm_sns         = Adc_Chip2.getChannel(4);
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
