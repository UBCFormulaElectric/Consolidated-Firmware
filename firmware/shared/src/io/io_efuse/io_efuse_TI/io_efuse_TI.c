#include "io_efuse_TI.h"
#include "hw_gpio.h"
#include <stdbool.h>
#include <assert.h>

static void io_TI_efuse_setChannel(const Efuse *channel, const bool enabled);
static bool io_TI_efuse_isChannelEnabled(const Efuse *channel);
static float io_TI_efuse_getChannelCurrent(const Efuse *channel);
static void io_TI_efuse_Reset(const Efuse *efuse);
static bool io_TI_efuse_pgood(const Efuse *efuse);

const TI_EfuseFunctons ti_efuse_functions = {
    .set_channel = io_TI_efuse_setChannel,
    .is_channel_enabled = io_TI_efuse_isChannelEnabled,
    .get_channel_current = io_TI_efuse_getChannelCurrent,
    .loadswitch_reset_set = NULL
    .reset_efuse = io_TI_efuse_Reset,
};

static void io_TI_efuse_setChannel(const Efuse *channel, const bool enabled)
{
    assert(channel->enable_gpio != NULL);
    hw_gpio_writePin(channel->enable_gpio, enabled);
}

static bool io_TI_efuse_isChannelEnabled(const Efuse *channel)
{
    assert(channel->enable_gpio != NULL);
    return hw_gpio_readPin(channel->enable_gpio);
}

static float io_TI_efuse_getChannelCurrent(const Efuse *channel)
{
    const AdcChannel *current_sense = channel->sns_adc_channel;
    assert(current_sense != NULL);
    return hw_adc_getVoltage(current_sense) * ADC_VOLTAGE_TO_CURRENT_A;
}

static void io_TI_efuse_Reset(const Efuse *efuse)
{
    assert(efuse->efuse != NULL);
    assert(efuse->efuse->enable_gpio != NULL);

    hw_gpio_writePin(efuse->efuse->enable_gpio, false);
    hw_gpio_writePin(efuse->efuse->enable_gpio, true);
    hw_gpio_writePin(efuse->efuse->enable_gpio, false);
}

static bool io_TI_efuse_pgood(const Efuse *efuse)
{
    assert(efuse->pgood != NULL);
    return hw_gpio_readPin(efuse->pgood);
}
