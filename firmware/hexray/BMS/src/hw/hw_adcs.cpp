#include "hw_adcs.hpp"
#include "main.h"

constexpr hw::AdcChip<7> adc1{ hadc1, htim3 };
constexpr hw::AdcChip<1> adc3{ hadc3, htim3 };

constexpr hw::Adc fan_isns       = adc1.getChannel(0);
constexpr hw::Adc ts_vsense_p    = adc1.getChannel(1);
constexpr hw::Adc ts_vsense_n    = adc1.getChannel(2);
constexpr hw::Adc shdn_sns       = adc1.getChannel(3);
constexpr hw::Adc emeter_tsns    = adc1.getChannel(4);
constexpr hw::Adc ts_isense_400a = adc1.getChannel(5);
constexpr hw::Adc ts_isense_50a  = adc1.getChannel(6);
constexpr hw::Adc aux_tsns       = adc3.getChannel(0);

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
