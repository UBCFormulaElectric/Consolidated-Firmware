#include "hw_adcs.hpp"
#include "main.h"

constexpr hw::AdcChip<6> adc1{ hadc1, htim2 };
constexpr hw::Adc        susp_fl   = adc1.getChannel(0);
constexpr hw::Adc        susp_fr   = adc1.getChannel(1);
constexpr hw::Adc        apps2     = adc1.getChannel(2);
constexpr hw::Adc        bps_f     = adc1.getChannel(3);
constexpr hw::Adc        str_angle = adc1.getChannel(4);
constexpr hw::Adc        apps1     = adc1.getChannel(5);

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == &adc1.gethadc())
        adc1.update_callback();
}
