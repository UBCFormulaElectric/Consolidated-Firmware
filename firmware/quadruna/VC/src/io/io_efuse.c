#include "io_efuses.h"

#include <assert.h>

#include "hw_gpios.h"
#include "hw_adcs.h"

typedef struct
{
    const Gpio       *enable_gpio;
    const Gpio       *stby_reset_gpio;
    const AdcChannel *cur_sns_adc_channel;
} EfuseConfig;

static const EfuseConfig configs[NUM_EFUSE_CHANNELS] = {
    [EFUSE_CHANNEL_SHDN] = {
        .enable_gpio = &shdn_pwr_en,
        .stby_reset_gpio = &fr_stby1,
        .cur_sns_adc_channel = &shdn_pwr_i_sns,
    },
    [EFUSE_CHANNEL_LV] = {
        .enable_gpio = &lv_pwr_en,
        .stby_reset_gpio = &fr_stby1,
        .cur_sns_adc_channel = &lv_pwr_i_sns,
    },
    [EFUSE_CHANNEL_PUMP] = {
        .enable_gpio = &pump_pwr_en,
        .stby_reset_gpio = &fr_stby2,
        .cur_sns_adc_channel = &pump_pwr_i_sns
    },
    [EFUSE_CHANNEL_AUX] = {
        .enable_gpio = &aux_pwr_en,
        .stby_reset_gpio = &fr_stby2,
        .cur_sns_adc_channel = &aux_pwr_i_sns
    },
    [EFUSE_CHANNEL_INV_R] = {
        .enable_gpio = &inv_r_pwr_en,
        .stby_reset_gpio = &fr_stby3,
        .cur_sns_adc_channel = &inv_r_pwr_i_sns
    },
    [EFUSE_CHANNEL_INV_L] = {
        .enable_gpio = &inv_l_pwr_en,
        .stby_reset_gpio = &fr_stby3,
        .cur_sns_adc_channel = &inv_l_pwr_i_sns
    },
    [EFUSE_CHANNEL_TELEM] = {
        .enable_gpio = &telem_pwr_en,
        .stby_reset_gpio = NULL,
        .cur_sns_adc_channel = NULL
    },
    [EFUSE_CHANNEL_BUZZER] = {
        .enable_gpio = &buzzer_pwr_en,
        .stby_reset_gpio = NULL,
        .cur_sns_adc_channel = NULL
    }
};

// TODO: Test ADC voltage -> output current transfer function
#define ADC_VOLTAGE_TO_CURRENT_A 1.720f
static bool enabled_channels[NUM_EFUSE_CHANNELS] = { false };

void io_efuse_setChannel(const EfuseChannel channel, const bool enabled)
{
    assert(channel < NUM_EFUSE_CHANNELS);

    enabled_channels[channel] = enabled;

    if (configs[channel].enable_gpio != NULL)
    {
        hw_gpio_writePin(configs[channel].enable_gpio, enabled);
    }
}

bool io_efuse_isChannelEnabled(const EfuseChannel channel)
{
    assert(channel < NUM_EFUSE_CHANNELS);

    return enabled_channels[channel];
}

float io_efuse_getChannelCurrent(const EfuseChannel channel)
{
    assert(channel < NUM_EFUSE_CHANNELS);
    const AdcChannel *a = configs[channel].cur_sns_adc_channel;
    if (a == NULL)
        return 0;
    return hw_adc_getVoltage(a) * ADC_VOLTAGE_TO_CURRENT_A;
}

void io_efuse_standbyReset(const EfuseChannel channel)
{
    assert(channel < NUM_EFUSE_CHANNELS);

    // Low pulse on standby reset line delatches faults
    hw_gpio_writePin(configs[channel].stby_reset_gpio, true);
    hw_gpio_writePin(configs[channel].stby_reset_gpio, false);
    hw_gpio_writePin(configs[channel].stby_reset_gpio, true);
}
