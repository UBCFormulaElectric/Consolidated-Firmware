#include "hw_adcs.hpp"
#include "main.h"

namespace hw::adc
{
AdcChip<NUM_ADC1_CHANNELS> adc1 = AdcChip<NUM_ADC1_CHANNELS>(&hadc1, &htim3);

AdcChip<NUM_ADC3_CHANNELS> adc3 = AdcChip<NUM_ADC3_CHANNELS>(&hadc3, &htim3);

Adc fan_isns       = Adc{ adc1.getChannel(0) };
Adc ts_vsense_p    = Adc{ adc1.getChannel(1) };
Adc ts_vsense_n    = Adc{ adc1.getChannel(2) };
Adc shdn_sns       = Adc{ adc1.getChannel(3) };
Adc emeter_tsns    = Adc{ adc1.getChannel(4) };
Adc ts_isense_400a = Adc{ adc1.getChannel(5) };
Adc ts_isense_50a  = Adc{ adc1.getChannel(6) };

Adc aux_tsns = Adc{ adc3.getChannel(0) };

void chipsInit(void)
{
    adc1.init();
    adc3.init();
}

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == &hadc1)
    {
        adc1.update_callback();
    }
    else if (hadc == &hadc3)
    {
        adc3.update_callback();
    }
}

} // namespace hw::adc
