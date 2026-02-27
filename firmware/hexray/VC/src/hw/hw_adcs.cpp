#include "hw_adcs.hpp"
#include "main.h"

namespace hw::adcs
{
constexpr AdcChip<6> Adc_Chip1{ hadc1, htim3 };
constexpr AdcChip<5> Adc_Chip2{ hadc2, htim3 };

constexpr Adc adc_front     = Adc_Chip1.getChannel(0);
constexpr Adc adc_r_inv     = Adc_Chip1.getChannel(1);
constexpr Adc adc_bms       = Adc_Chip1.getChannel(2);
constexpr Adc adc_rl_pump   = Adc_Chip1.getChannel(3);
constexpr Adc adc_l_rad_fan = Adc_Chip1.getChannel(4);
constexpr Adc adc_rr_pump   = Adc_Chip1.getChannel(5);
constexpr Adc adc_dam       = Adc_Chip2.getChannel(0);
constexpr Adc adc_r_rad_fan = Adc_Chip2.getChannel(1);
constexpr Adc adc_misc_fuse = Adc_Chip2.getChannel(2);
constexpr Adc adc_f_inv     = Adc_Chip2.getChannel(3);
constexpr Adc adc_rsm       = Adc_Chip2.getChannel(4);
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
