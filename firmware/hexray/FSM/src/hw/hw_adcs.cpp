#include "hw_adc.hpp"
#include "hw_adcs.hpp"
#include "main.h"

#define NUM_ADC_CHANNELS 6 // Number of channels being measured by adc1

namespace hw::adcs
{
AdcChip<NUM_ADC_CHANNELS> adc1 = AdcChip<NUM_ADC_CHANNELS>(&hadc1, &htim2);

void chipsInit(void)
{
    adc1.init();
} // Do I need this function? Can't I just call adc1.init() directly?

Adc susp_fl   = Adc(adc1.getChannel(0));
Adc susp_fr   = Adc(adc1.getChannel(1));
Adc apps2     = Adc(adc1.getChannel(2));
Adc bps_f     = Adc(adc1.getChannel(3));
Adc str_angle = Adc(adc1.getChannel(4));
Adc apps1     = Adc(adc1.getChannel(5));
} // namespace hw::adcs

extern "C"
{
    void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
    {
        if (hadc == adc1.hadc)
            adc1.update_callback();
    }
}
