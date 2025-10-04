#include "io_efuse.h"
#include "hw_gpio.h"
#include <stdbool.h>
#include <assert.h>

void io_efuse_setChannel(const Efuse *channel, const bool enabled)
{
    assert(channel != NULL);

    channel->efuse_functions->set_channel(channel, enabled);
}

bool io_efuse_isChannelEnabled(const Efuse *channel)
{
    assert(channel->enable_gpio != NULL);
    return chann;
}

float io_efuse_getChannelCurrent(const Efuse *channel)
{
    const AdcChannel *current_sense = channel->sns_adc_channel;
    assert(current_sense != NULL);
    return hw_adc_getVoltage(current_sense) * ADC_VOLTAGE_TO_CURRENT_A;
}

void io_STefuse_reset_set(const ST_efuse *efuse, const bool set)
{
    assert(efuse->stby_reset_gpio != NULL);
    hw_gpio_writePin(efuse->stby_reset_gpio, set);
}

void io_STefuse_Reset(const ST_efuse *efuse)
{
    assert(efuse->stby_reset_gpio != NULL);
    hw_gpio_writePin(efuse->stby_reset_gpio, false);
    hw_gpio_writePin(efuse->stby_reset_gpio, true);
    hw_gpio_writePin(efuse->stby_reset_gpio, false);
}

void io_TIefuse_Reset(const TI_efuse *efuse)
{
    assert(efuse->efuse != NULL);
    assert(efuse->efuse->enable_gpio != NULL);

    hw_gpio_writePin(efuse->efuse->enable_gpio, false);
    hw_gpio_writePin(efuse->efuse->enable_gpio, true);
    hw_gpio_writePin(efuse->efuse->enable_gpio, false);
}

bool io_TIefuse_pgood(const TI_efuse *efuse)
{
    assert(efuse->pgood != NULL);
    return hw_gpio_readPin(efuse->pgood);
}
