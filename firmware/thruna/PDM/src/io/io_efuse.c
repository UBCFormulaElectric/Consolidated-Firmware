#include "io_efuse.h"
#include <stdint.h>
#include <assert.h>

// TODO: Test ADC voltage -> output current transfer function
#define ADC_VOLTAGE_TO_CURRENT_A 1.720f

static const EfuseConfig * configs;
static bool                enabled_channels[NUM_EFUSE_CHANNELS];

void io_efuse_init(const EfuseConfig efuse_configs[NUM_EFUSE_CHANNELS])
{
    configs = efuse_configs;
}

void io_efuse_setChannel(EfuseChannel channel, bool enabled)
{
    assert(channel < NUM_EFUSE_CHANNELS);

    enabled_channels[channel] = enabled;
    hw_gpio_writePin(&configs[channel].enable_gpio, enabled);
}

bool io_efuse_isChannelEnabled(EfuseChannel channel)
{
    assert(channel < NUM_EFUSE_CHANNELS);

    return enabled_channels[channel];
}

float io_efuse_getChannelCurrent(EfuseChannel channel)
{
    assert(channel < NUM_EFUSE_CHANNELS);

    return hw_adc_getVoltage(configs[channel].cur_sns_adc_channel) * ADC_VOLTAGE_TO_CURRENT_A;
}

void io_efuse_standbyReset(EfuseChannel channel)
{
    assert(channel < NUM_EFUSE_CHANNELS);

    // Low pulse on standby reset line delatches faults
    hw_gpio_writePin(&configs[channel].stby_reset_gpio, true);
    hw_gpio_writePin(&configs[channel].stby_reset_gpio, false);
    hw_gpio_writePin(&configs[channel].stby_reset_gpio, true);
}