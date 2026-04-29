#include "hw_adcs.hpp"
#include "main.h"

namespace hw::adcs
{
constexpr AdcChip<6> adc1{ hadc1, htim2 };

constexpr Adc susp_fl   = adc1.getChannel(0);
constexpr Adc susp_fr   = adc1.getChannel(1);
constexpr Adc apps2     = adc1.getChannel(2);
constexpr Adc bps_f     = adc1.getChannel(3);
constexpr Adc str_angle = adc1.getChannel(4);
constexpr Adc apps1     = adc1.getChannel(5);
} // namespace hw::adcs

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc == &hw::adcs::adc1.gethadc())
        hw::adcs::adc1.update_callback();
}
