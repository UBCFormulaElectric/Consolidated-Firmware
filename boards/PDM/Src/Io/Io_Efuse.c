#include <stdint.h>
#include <assert.h>
#include "main.h"
#include "Io_Efuse.h"
#include "Io_Hal.h"
#include "Io_Adc.h"

// TODO: Test ADC voltage -> output current transfer function
#define ADC_VOLTAGE_TO_CURRENT_A 1.720f 

typedef struct
{
    uint16_t      in_gpio_pin;
    GPIO_TypeDef *in_gpio_port;
    uint16_t      cur_sns_adc_channel;
    uint16_t      stby_reset_gpio_pin;
    GPIO_TypeDef *stby_reset_gpio_port;
} EfuseChannelIoConfig;

static EfuseChannelIoConfig config[NUM_EFUSE_CHANNELS] = {
    [EFUSE_CHANNEL_AIR] = {
        .in_gpio_pin = EN_AIR_Pin,
        .in_gpio_port = EN_AIR_GPIO_Port,
        .cur_sns_adc_channel = ADC1_CHANNEL_9,
        .stby_reset_gpio_pin = FR_STBY_AIR_LVPWR_Pin,
        .stby_reset_gpio_port = FR_STBY_AIR_LVPWR_GPIO_Port,
    },
    [EFUSE_CHANNEL_LVPWR] = {
        .in_gpio_pin = EN_LVPWR_Pin,
        .in_gpio_port = EN_LVPWR_GPIO_Port,
        .cur_sns_adc_channel = ADC1_CHANNEL_8,
        .stby_reset_gpio_pin = FR_STBY_AIR_LVPWR_Pin,
        .stby_reset_gpio_port = FR_STBY_AIR_LVPWR_GPIO_Port,
    },
    [EFUSE_CHANNEL_EMETER] = {
        .in_gpio_pin = EN_EMETER_Pin,
        .in_gpio_port = EN_EMETER_GPIO_Port,
        .cur_sns_adc_channel = ADC1_CHANNEL_15,
        .stby_reset_gpio_pin = FR_STBY_EMETER_AUX_Pin,
        .stby_reset_gpio_port = FR_STBY_EMETER_AUX_GPIO_Port,
    },
    [EFUSE_CHANNEL_AUX] = {
        .in_gpio_pin = EN_AUX_Pin,
        .in_gpio_port = EN_AUX_GPIO_Port,
        .cur_sns_adc_channel = ADC1_CHANNEL_14,
        .stby_reset_gpio_pin = FR_STBY_EMETER_AUX_Pin,
        .stby_reset_gpio_port = FR_STBY_EMETER_AUX_GPIO_Port,
    },
    [EFUSE_CHANNEL_DRS] = {
        .in_gpio_pin = EN_DRS_Pin,
        .in_gpio_port = EN_DRS_GPIO_Port,
        .cur_sns_adc_channel = ADC1_CHANNEL_7,
        .stby_reset_gpio_pin = FR_STBY_DRS_FAN_Pin,
        .stby_reset_gpio_port = FR_STBY_DRS_FAN_GPIO_Port,
    },
    [EFUSE_CHANNEL_FAN] = {
        .in_gpio_pin = EN_FAN_Pin,
        .in_gpio_port = EN_FAN_GPIO_Port,
        .cur_sns_adc_channel = ADC1_CHANNEL_6,
        .stby_reset_gpio_pin = FR_STBY_DRS_FAN_Pin,
        .stby_reset_gpio_port = FR_STBY_DRS_FAN_GPIO_Port,
    },
    [EFUSE_CHANNEL_DI_LHS] = {
        .in_gpio_pin = EN_DI_LHS_Pin,
        .in_gpio_port = EN_DI_LHS_GPIO_Port,
        .cur_sns_adc_channel = ADC1_CHANNEL_5,
        .stby_reset_gpio_pin = FR_STBY_DIS_Pin,
        .stby_reset_gpio_port = FR_STBY_DIS_GPIO_Port,
    },
    [EFUSE_CHANNEL_DI_RHS] = {
        .in_gpio_pin = EN_DI_RHS_Pin,
        .in_gpio_port = EN_DI_RHS_GPIO_Port,
        .cur_sns_adc_channel = ADC1_CHANNEL_4,
        .stby_reset_gpio_pin = FR_STBY_DIS_Pin,
        .stby_reset_gpio_port = FR_STBY_DIS_GPIO_Port,
    }
};

static bool enabled_channels[NUM_EFUSE_CHANNELS];

void Io_Efuse_SetChannel(EfuseChannel channel, bool enabled)
{
    assert(channel < NUM_EFUSE_CHANNELS);

    enabled_channels[channel] = enabled;
    HAL_GPIO_WritePin(
        config[channel].in_gpio_port, config[channel].in_gpio_pin, enabled ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

bool Io_Efuse_IsChannelEnabled(EfuseChannel channel)
{
    assert(channel < NUM_EFUSE_CHANNELS);

    return enabled_channels[channel];
}

float Io_Efuse_GetChannelCurrent(EfuseChannel channel)
{
    assert(channel < NUM_EFUSE_CHANNELS);

    return Io_Adc_GetChannelVoltage(config[channel].cur_sns_adc_channel) * ADC_VOLTAGE_TO_CURRENT_A;
}

void Io_Efuse_DelatchChannelFaults(EfuseChannel channel)
{
    assert(channel < NUM_EFUSE_CHANNELS);

    // Low pulse on standby reset line delatches faults
    HAL_GPIO_WritePin(config[channel].stby_reset_gpio_port, config[channel].stby_reset_gpio_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(config[channel].stby_reset_gpio_port, config[channel].stby_reset_gpio_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(config[channel].stby_reset_gpio_port, config[channel].stby_reset_gpio_pin, GPIO_PIN_SET);
}
