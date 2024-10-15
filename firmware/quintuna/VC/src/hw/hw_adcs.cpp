#include "hw_adc.h"
#include <assert.h>
#include "hw_hal.h"
#include "hw_adcConversions.h"
#include "hw_utils.h"

namespace hw::adc
{
const AdcChip<NUM_ADC_CHANNELS> adc1{ &hadc1 };
const AdcChip<NUM_ADC_CHANNELS> adc3{ &hadc3 };

const Adc regen_3v3_adc{ adc1.getChannel(ADC1_IN14_REGEN) };

const Adc _24v_acc_sns_adc{ adc1.getChannel(ADC1_IN3_24V_ACC_SENSE) };
const Adc lv_pwr_i_sns_adc{ adc1.getChannel(ADC1_IN4_LV_PWR_I_SNS) };
const Adc acc_i_sns_adc{ adc1.getChannel(ADC1_IN5_ACC_I_SENSE) };
const Adc _22v_boost_sns_adc{ adc1.getChannel(ADC1_IN7_22V_BOOST_SENSE) };
const Adc inv_r_pwr_i_sns_adc{ adc1.getChannel(ADC1_IN10_INV_R_PWR_I_SNS) };
const Adc inv_l_pwr_i_sns_adc{ adc1.getChannel(ADC1_IN11_INV_L_PWR_I_SNS) };
const Adc bat_i_sns_adc{ adc1.getChannel(ADC1_IN14_BAT_I_SNS) };
const Adc shdn_pwr_i_sns_adc{ adc1.getChannel(ADC1_IN18_SHDN_PWR_I_SNS) };
const Adc vbat_sense_adc{ adc1.getChannel(ADC1_IN19_VBAT_SENSE) };
const Adc aux_pwr_i_sns_adc{ adc3.getChannel(ADC3_IN0_AUX_PWR_I_SNS) };
const Adc pump_pwr_i_sns_adc{ adc3.getChannel(ADC3_IN1_PUMP_PWR_I_SNS) };
} // namespace hw::adc

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    hw::adc::hadc.update_callback();
}
