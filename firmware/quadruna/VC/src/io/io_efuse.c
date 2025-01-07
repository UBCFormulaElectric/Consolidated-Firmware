#include "io_efuse.h"
#include <assert.h>

// TODO: Test ADC voltage -> output current transfer function
#define ADC_VOLTAGE_TO_CURRENT_A 1.720f

static const EfuseConfig *configs = NULL;

void io_efuse_init(const EfuseConfig efuse_configs[NUM_EFUSE_CHANNELS])
{
    configs = efuse_configs;
}

static bool enabled_channels[NUM_EFUSE_CHANNELS] = { false };

void io_efuse_setChannel(EfuseChannel channel, bool enabled)
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

void io_efuse_standbyReset(EfuseChannel channel)
{
    assert(channel < NUM_EFUSE_CHANNELS);

    // Low pulse on standby reset line delatches faults
    hw_gpio_writePin(configs[channel].stby_reset_gpio, true);
    hw_gpio_writePin(configs[channel].stby_reset_gpio, false);
    hw_gpio_writePin(configs[channel].stby_reset_gpio, true);
}
