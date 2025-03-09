#include "io_loadswitch.h"

const Efuse bms_en_switch      = { .enable_gpio = &bms_en, .cur_sns_adc_channel = &bms_i_sns };
const Efuse front_en_switch    = { .enable_gpio = &front_en, .cur_sns_adc_channel = &front_i_sns };
const Efuse rsm_en_switch      = { .enable_gpio = &rsm_en, .cur_sns_adc_channel = &rsm_i_sns };
const Efuse inv_r_pwr_switch   = { .enable_gpio = &r_inv_en, .cur_sns_adc_channel = &inv_r_pwr_i_sns };
const Efuse pump_rl_pwr_switch = { .enable_gpio = &rl_pump_en, .cur_sns_adc_channel = &pump_rl_pwr_i_sns };
const Efuse pump_rr_pwr_switch = { .enable_gpio = &rr_pump_en, .cur_sns_adc_channel = &pump_rr_pwr_i_sns };
const Efuse pump_f_pwr_switch  = { .enable_gpio = &f_pump_en, .cur_sns_adc_channel = &pump_f_pwr_i_sns };
const Efuse l_rad_fan_switch   = { .enable_gpio = &l_rad_fan_en, .cur_sns_adc_channel = &l_rad_fan_i_sns };
const Efuse r_rad_fan_switch   = { .enable_gpio = &r_rad_fan_en, .cur_sns_adc_channel = &r_rad_fan_i_sns };
const Efuse extra_switch       = { .enable_gpio = &extra_en, .cur_sns_adc_channel = &extra_i_sns };
const Efuse inv_l_pwr_switch   = { .enable_gpio = &l_inv_en, .cur_sns_adc_channel = &inv_l_pwr_i_sns };
const Efuse dam_en_switch      = { .enable_gpio = &dam_en, .cur_sns_adc_channel = &dam_i_sns };

const LoadSwitch inverter_loadswitch   = { .efuse1          = &inv_l_pwr_switch,
                                           .efuse2          = &inv_r_pwr_switch,
                                           .stby_reset_gpio = &fr_stby_inv };
const LoadSwitch rear_pumps_loadswitch = { .efuse1          = &pump_rl_pwr_switch,
                                           .efuse2          = &pump_rr_pwr_switch,
                                           .stby_reset_gpio = &fr_stby_rear_pump };
const LoadSwitch front_pump_loadswitch = { .efuse1          = &pump_f_pwr_switch,
                                           .efuse2          = &extra_switch,
                                           .stby_reset_gpio = &fr_stby_f_pump };
const LoadSwitch rad_fan_loadswitch    = { .efuse1          = &l_rad_fan_switch,
                                           .efuse2          = &r_rad_fan_switch,
                                           .stby_reset_gpio = &fr_stby_rad };
const LoadSwitch front_loadswitch      = { .efuse1          = &bms_en_switch,
                                           .efuse2          = &front_en_switch,
                                           .stby_reset_gpio = &fr_stby_front };
const LoadSwitch rear_loadswitch       = { .efuse1          = &rsm_en_switch,
                                           .efuse2          = &dam_en_switch,
                                           .stby_reset_gpio = &fr_stby_rear };

#define ADC_VOLTAGE_TO_CURRENT_A 1.720f

void io_loadswitch_setChannel(const Efuse *channel, const bool enabled)
{
    if (channel->enable_gpio != NULL)
    {
        hw_gpio_writePin(channel->enable_gpio, enabled);
    }
}

bool io_loadswitch_isChannelEnabled(const Efuse *channel)
{
    return hw_gpio_readPin(channel->enable_gpio);
}

float io_loadswitch_getChannelCurrent(const Efuse *channel)
{
    const AdcChannel *current_sense = channel->cur_sns_adc_channel;
    if (current_sense == NULL)
    {
        return 0;
    }
    return hw_adc_getVoltage(current_sense) * ADC_VOLTAGE_TO_CURRENT_A;
}

void io_loadswitch_standbyReset(const LoadSwitch *loadswitch)
{
    hw_gpio_writePin(loadswitch->stby_reset_gpio, true);
    hw_gpio_writePin(loadswitch->stby_reset_gpio, false);
    hw_gpio_writePin(loadswitch->stby_reset_gpio, false);
}