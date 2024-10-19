#include "hw_adcs.h"
#include <assert.h>
#include "hw_hal.h"
#include "hw_adcConversions.h"
#include "main.h"

namespace hw::adc
{
    const AdcChip<NUM_ADC_CHANNELS> adc1{ &hadc1 };
    const Adc                       acc_fan_i_sns{ adc1.getChannel(ADC1_IN15_ACC_FAN_I_SNS) }; 
    const Adc                       rad_fan_i_sns{ adc1.getChannel(ADC1_IN14_RAD_FAN_I_SNS) };   
    const Adc                       coolantpressure1_3v3{ adc1.getChannel(ADC1_IN12_COOLANT_PRESSURE_1) };  
    const Adc                       coolantpressure2_3v3{ adc1.getChannel(ADC1_IN11_COOLANT_PRESSURE_2) };  
    const Adc                       lc3_out{ adc1.getChannel(ADC1_IN10_LC3_OUT) };  
    const Adc                       susp_travel_rr_3v3{ adc1.getChannel(ADC1_IN4_SUSP_TRAVEL_RR) };  
    const Adc                       susp_travel_rl_3v3{ adc1.getChannel(ADC1_IN1_SUSP_TRAVEL_RL) };  
    const Adc                       coolanttemp2_3v3{ adc1.getChannel(ADC1_IN3_COOLANT_TEMP_2) };  
    const Adc                       coolanttemp1_3v3{ adc1.getChannel(ADC1_IN2_COOLANT_TEMP_1) };
    const Adc                       regen_3v3_lc3_out{ adc1.getChannel(ADC1_IN0_LC4_OUT) };
    
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    UNUSED(hadc);
    hw::adc::adc1.update_callback();
}