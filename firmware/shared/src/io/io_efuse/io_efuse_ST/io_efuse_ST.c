#include "io_efuse_ST.h"
#include "io_efuse/io_efuse_datatypes.h"
#include "hw_gpio.h"
#include <stdbool.h>
#include <assert.h>

/**
 * ST Efuse Datasheet:
 * https://octopart.com/datasheet/vnd5t100ajtr-e-stmicroelectronics-21218840
 */

static float   NOMINAL_V            = 3.0f;
static uint8_t ST_EFUSE_FAULT_FLAGS = 0x3F;

static void  io_efuse_ST_setChannel(const Efuse *channel, bool enabled);
static bool  io_efuse_ST_isChannelEnabled(const Efuse *channel);
static float io_efuse_ST_getChannelCurrent(const Efuse *channel);
static void  io_efuse_ST_reset_set(const Efuse *channel, bool set);
static void  io_efuse_ST_reset(const Efuse *channel);
static bool  io_efuse_ST_ok(const Efuse *efuse);

const EfuseFunctions st_efuse_functions = { .set_channel          = io_efuse_ST_setChannel,
                                            .is_channel_enabled   = io_efuse_ST_isChannelEnabled,
                                            .get_channel_current  = io_efuse_ST_getChannelCurrent,
                                            .loadswitch_reset_set = io_efuse_ST_reset_set,
                                            .reset_efuse          = io_efuse_ST_reset,
                                            .pgood                = NULL,
                                            .efuse_ok             = io_efuse_ST_ok };

static void io_efuse_ST_setChannel(const Efuse *channel, bool enabled)
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

static void io_efuse_ST_reset_set(const Efuse *channel, bool set)
{
    assert(channel->st->stby_reset_gpio != NULL);
    hw_gpio_writePin(channel->st->stby_reset_gpio, set);
}

static void io_efuse_ST_reset(const Efuse *channel)
{
    assert(channel->st->stby_reset_gpio != NULL);
    hw_gpio_writePin(channel->st->stby_reset_gpio, false);
    hw_gpio_writePin(channel->st->stby_reset_gpio, true);
    hw_gpio_writePin(channel->st->stby_reset_gpio, false);
}

static bool io_efuse_ST_ok(const Efuse *efuse)
{
    assert(efuse != NULL);

    float voltage        = io_efuse_ST_getChannelCurrent(efuse) / ADC_VOLTAGE_TO_CURRENT_A;
    bool  channelEnabled = io_efuse_ST_isChannelEnabled(efuse);

    // Setting faults for st efuse
    // TODO: these probably are not correct they must be fixed
    efuse->st->faults.flags.under_voltage                 = (voltage <= 0.0f);
    efuse->st->faults.flags.short_to_vbat                 = (voltage >= NOMINAL_V && channelEnabled);
    efuse->st->faults.flags.overload                      = (voltage > NOMINAL_V && channelEnabled);
    efuse->st->faults.flags.ovt_stp                       = (voltage >= NOMINAL_V && channelEnabled);
    efuse->st->faults.flags.negative_output_voltage_clamp = (voltage <= 0.0f && !channelEnabled);
    efuse->st->faults.flags.open_load_off_stat            = (voltage >= NOMINAL_V);

    // check if any flag is set, then return the status
    uint8_t flags = efuse->st->faults.raw & ST_EFUSE_FAULT_FLAGS;

    return !(flags > 0);
}
