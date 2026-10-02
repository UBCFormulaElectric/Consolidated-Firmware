#include "hw_adcs.hpp"
#include "main.h"

constexpr hw::adcchip<6> adc1{ hadc1, htim2 };
constexpr hw::adc        susp_fl   = adc1.getChannel(0);
constexpr hw::adc        susp_fr   = adc1.getChannel(1);
constexpr hw::adc        apps2     = adc1.getChannel(2);
constexpr hw::adc        bps_f     = adc1.getChannel(3);
constexpr hw::adc        str_angle = adc1.getChannel(4);
constexpr hw::adc        apps1     = adc1.getChannel(5);

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == &adc1.gethadc())
        adc1.update_callback();
}
