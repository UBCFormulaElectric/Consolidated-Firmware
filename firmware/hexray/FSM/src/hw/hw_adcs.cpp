#include "hw_adcs.hpp"
extern "C"
{
#include "main.h"
}

namespace hw::adcs
{
constexpr int             NUM_ADC_CHANNELS = 6;
AdcChip<NUM_ADC_CHANNELS> adc1(&hadc1, &htim2);

void chipsInit(void)
{
    adc1.init();
}

Adc susp_fl   = Adc(adc1.getChannel(0));
Adc susp_fr   = Adc(adc1.getChannel(1));
Adc apps2     = Adc(adc1.getChannel(2));
Adc bps_f     = Adc(adc1.getChannel(3));
Adc str_angle = Adc(adc1.getChannel(4));
Adc apps1     = Adc(adc1.getChannel(5));
} // namespace hw::adcs

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == hw::adcs::adc1.gethadc())
        hw::adcs::adc1.update_callback();
}
