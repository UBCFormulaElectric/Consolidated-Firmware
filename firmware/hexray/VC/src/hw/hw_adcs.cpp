#include "hw_adcs.hpp"
#include "main.h"

constexpr hw::adcchip<6> Adc_Chip1{ hadc1, htim3 };
constexpr hw::adcchip<5> Adc_Chip2{ hadc2, htim3 };

// The channel index is the DMA buffer slot, which is the CubeMX regular rank minus one. Keep these
// in rank order and matched to the pin each rank's ADC channel is wired to in VC.ioc.
constexpr hw::adc adc_rr_pump   = Adc_Chip1.getChannel(0); // rank 1, ADC1_INP8,  PC5, RR_PUMP_I_SNS
constexpr hw::adc adc_rl_pump   = Adc_Chip1.getChannel(1); // rank 2, ADC1_INP9,  PB0, RL_PUMP_I_SNS
constexpr hw::adc adc_l_rad_fan = Adc_Chip1.getChannel(2); // rank 3, ADC1_INP10, PC0, L_RAD_FAN_I_SNS
constexpr hw::adc adc_bms       = Adc_Chip1.getChannel(3); // rank 4, ADC1_INP15, PA3, BMS_SNS
constexpr hw::adc adc_front     = Adc_Chip1.getChannel(4); // rank 5, ADC1_INP16, PA0, FRONT_SNS
constexpr hw::adc adc_r_inv     = Adc_Chip1.getChannel(5); // rank 6, ADC1_INP17, PA1, R_INV_SNS
constexpr hw::adc adc_dam       = Adc_Chip2.getChannel(0); // rank 1, ADC2_INP4,  PC4, DAM_SNS
constexpr hw::adc adc_r_rad_fan = Adc_Chip2.getChannel(1); // rank 2, ADC2_INP5,  PB1, R_RAD_FAN_I_SNS
constexpr hw::adc adc_misc_fuse = Adc_Chip2.getChannel(2); // rank 3, ADC2_INP11, PC1, MISC_FUSE_SNS
constexpr hw::adc adc_f_inv     = Adc_Chip2.getChannel(3); // rank 4, ADC2_INP14, PA2, F_INV_SNS
constexpr hw::adc adc_rsm       = Adc_Chip2.getChannel(4); // rank 5, ADC2_INP18, PA4, RSM_SNS

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
