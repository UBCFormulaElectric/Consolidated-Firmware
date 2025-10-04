#include "io_efuse_ST.h"
#include "hw_gpio.h"
#include <stdbool.h>
#include <assert.h>

static void  io_efuse_ST_setChannel(const Efuse *channel, const bool enabled);
static bool  io_efuse_ST_isChannelEnabled(const Efuse *channel);
static float io_efuse_ST_getChannelCurrent(const Efuse *channel);
static void  io_efuse_ST_reset_set(const Efuse *channel, const bool set);
static void  io_efuse_ST_Reset(const Efuse *channel);

const EfuseFunctons st_efuse_functions = { .set_channel          = io_efuse_ST_setChannel,
                                           .is_channel_enabled   = io_efuse_ST_isChannelEnabled,
                                           .get_channel_current  = io_efuse_ST_getChannelCurrent,
                                           .loadswitch_reset_set = io_efuse_ST_reset_set,
                                           .reset_efuse          = io_efuse_ST_Reset };

static void io_efuse_ST_setChannel(const Efuse *channel, const bool enabled)
{
    assert(channel->enable_gpio != NULL);
    hw_gpio_writePin(channel->enable_gpio, enabled);
}

static bool io_efuse_ST_isChannelEnabled(const Efuse *channel)
{
    assert(channel->enable_gpio != NULL);
    return hw_gpio_readPin(channel->enable_gpio);
}

static float io_efuse_ST_getChannelCurrent(const Efuse *channel)
{
    const AdcChannel *current_sense = channel->sns_adc_channel;
    assert(current_sense != NULL);
    return hw_adc_getVoltage(current_sense) * ADC_VOLTAGE_TO_CURRENT_A;
}

static void io_efuse_ST_reset_set(const Efuse *channel, const bool set)
{
    assert(channel->st->stby_reset_gpio != NULL);
    hw_gpio_writePin(channel->st->stby_reset_gpio, set);
}

static void io_efuse_ST_Reset(const Efuse *channel)
{
    assert(channel->st->stby_reset_gpio != NULL);
    hw_gpio_writePin(channel->st->stby_reset_gpio, false);
    hw_gpio_writePin(channel->st->stby_reset_gpio, true);
    hw_gpio_writePin(channel->st->stby_reset_gpio, false);
}