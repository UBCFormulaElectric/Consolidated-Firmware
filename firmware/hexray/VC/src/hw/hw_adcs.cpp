#include "hw_adcs.hpp"
#include "main.h"

constexpr hw::adcchip<6> Adc_Chip1{ hadc1, htim3 };
constexpr hw::adcchip<5> Adc_Chip2{ hadc2, htim3 };

constexpr hw::Adc adc_front     = Adc_Chip1.getChannel(0);
constexpr hw::Adc adc_r_inv     = Adc_Chip1.getChannel(1);
constexpr hw::Adc adc_bms       = Adc_Chip1.getChannel(2);
constexpr hw::Adc adc_rl_pump   = Adc_Chip1.getChannel(3);
constexpr hw::Adc adc_l_rad_fan = Adc_Chip1.getChannel(4);
constexpr hw::Adc adc_rr_pump   = Adc_Chip1.getChannel(5);
constexpr hw::Adc adc_dam       = Adc_Chip2.getChannel(0);
constexpr hw::Adc adc_r_rad_fan = Adc_Chip2.getChannel(1);
constexpr hw::Adc adc_misc_fuse = Adc_Chip2.getChannel(2);
constexpr hw::Adc adc_f_inv     = Adc_Chip2.getChannel(3);
constexpr hw::Adc adc_rsm       = Adc_Chip2.getChannel(4);

extern "C"
{
    void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
    {
        if (hadc == &hadc1)
        {
            Adc_Chip1.update_callback();
        }
        else if (hadc == &hadc2)
        {
            Adc_Chip2.update_callback();
        }
    }
}
