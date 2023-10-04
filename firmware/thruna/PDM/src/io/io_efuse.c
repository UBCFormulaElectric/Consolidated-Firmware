#include "io_efuse.h"
#include <stdint.h>
#include <assert.h>
#include "main.h"

// TODO: Test ADC voltage -> output current transfer function
#define ADC_VOLTAGE_TO_CURRENT_A 1.720f

// typedef struct
// {
//     uint16_t      in_gpio_pin;
//     GPIO_TypeDef *in_gpio_port;
//     uint16_t      cur_sns_adc_channel;
//     uint16_t      stby_reset_gpio_pin;
//     GPIO_TypeDef *stby_reset_gpio_port;
// } EfuseChannelIoConfig;

// static EfuseChannelIoConfig config[NUM_EFUSE_CHANNELS] = {
//     [EFUSE_CHANNEL_AIR] = {
//         .in_gpio_pin = EN_AIR_Pin,
//         .in_gpio_port = EN_AIR_GPIO_Port,
//         .cur_sns_adc_channel = ADC1_CHANNEL_9,
//         .stby_reset_gpio_pin = FR_STBY_AIR_LVPWR_Pin,
//         .stby_reset_gpio_port = FR_STBY_AIR_LVPWR_GPIO_Port,
//     },
//     [EFUSE_CHANNEL_LVPWR] = {
//         .in_gpio_pin = EN_LVPWR_Pin,
//         .in_gpio_port = EN_LVPWR_GPIO_Port,
//         .cur_sns_adc_channel = ADC1_CHANNEL_8,
//         .stby_reset_gpio_pin = FR_STBY_AIR_LVPWR_Pin,
//         .stby_reset_gpio_port = FR_STBY_AIR_LVPWR_GPIO_Port,
//     },
//     [EFUSE_CHANNEL_EMETER] = {
//         .in_gpio_pin = EN_EMETER_Pin,
//         .in_gpio_port = EN_EMETER_GPIO_Port,
//         .cur_sns_adc_channel = ADC1_CHANNEL_15,
//         .stby_reset_gpio_pin = FR_STBY_EMETER_AUX_Pin,
//         .stby_reset_gpio_port = FR_STBY_EMETER_AUX_GPIO_Port,
//     },
//     [EFUSE_CHANNEL_AUX] = {
//         .in_gpio_pin = EN_AUX_Pin,
//         .in_gpio_port = EN_AUX_GPIO_Port,
//         .cur_sns_adc_channel = ADC1_CHANNEL_14,
//         .stby_reset_gpio_pin = FR_STBY_EMETER_AUX_Pin,
//         .stby_reset_gpio_port = FR_STBY_EMETER_AUX_GPIO_Port,
//     },
//     [EFUSE_CHANNEL_DRS] = {
//         .in_gpio_pin = EN_DRS_Pin,
//         .in_gpio_port = EN_DRS_GPIO_Port,
//         .cur_sns_adc_channel = ADC1_CHANNEL_7,
//         .stby_reset_gpio_pin = FR_STBY_DRS_FAN_Pin,
//         .stby_reset_gpio_port = FR_STBY_DRS_FAN_GPIO_Port,
//     },
//     [EFUSE_CHANNEL_FAN] = {
//         .in_gpio_pin = EN_FAN_Pin,
//         .in_gpio_port = EN_FAN_GPIO_Port,
//         .cur_sns_adc_channel = ADC1_CHANNEL_6,
//         .stby_reset_gpio_pin = FR_STBY_DRS_FAN_Pin,
//         .stby_reset_gpio_port = FR_STBY_DRS_FAN_GPIO_Port,
//     },
//     [EFUSE_CHANNEL_DI_LHS] = {
//         .in_gpio_pin = EN_DI_LHS_Pin,
//         .in_gpio_port = EN_DI_LHS_GPIO_Port,
//         .cur_sns_adc_channel = ADC1_CHANNEL_5,
//         .stby_reset_gpio_pin = FR_STBY_DIS_Pin,
//         .stby_reset_gpio_port = FR_STBY_DIS_GPIO_Port,
//     },
//     [EFUSE_CHANNEL_DI_RHS] = {
//         .in_gpio_pin = EN_DI_RHS_Pin,
//         .in_gpio_port = EN_DI_RHS_GPIO_Port,
//         .cur_sns_adc_channel = ADC1_CHANNEL_4,
//         .stby_reset_gpio_pin = FR_STBY_DIS_Pin,
//         .stby_reset_gpio_port = FR_STBY_DIS_GPIO_Port,
//     }
// };

void io_efuse_init(EFuse* efuse, const EfuseConfig* config)
{
    assert(efuse);
    assert(config);
    efuse->config = config;
    efuse->enabled = false;
}

void io_efuse_setChannel(EFuse* efuse, bool enabled)
{
    assert(efuse);
    hw_gpio_writePin(&efuse->config->enable_gpio, enabled);
    efuse->enabled = enabled;
}

bool io_efuse_isChannelEnabled(EFuse* efuse)
{
    assert(efuse);
    return efuse->enabled;
}

float io_efuse_getChannelCurrent(EFuse* efuse)
{
    assert(efuse);
    return hw_adc_getVoltage(efuse->config->isense_adc_channel) * ADC_VOLTAGE_TO_CURRENT_A;
}

void io_efuse_standbyReset(EFuse* efuse)
{
    assert(efuse);

    // Low pulse on standby reset line delatches faults.
    hw_gpio_writePin(&efuse->config->stby_reset_gpio, true);
    hw_gpio_writePin(&efuse->config->stby_reset_gpio, false);
    hw_gpio_writePin(&efuse->config->stby_reset_gpio, true);
}
