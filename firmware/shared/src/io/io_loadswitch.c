#include "io_loadswitch.h"
#include "hw_gpio.h"
#include <stdbool.h>
#include <assert.h>

void io_loadswitch_setChannel(const Efuse *channel, const bool enabled)
{
    assert(channel->enable_gpio != NULL);
    hw_gpio_writePin(channel->enable_gpio, enabled);
}

bool io_loadswitch_isChannelEnabled(const Efuse *channel)
{
    assert(channel->enable_gpio != NULL);
    return hw_gpio_readPin(channel->enable_gpio);
}

float io_loadswitch_getChannelCurrent(const Efuse *channel)
{
    const AdcChannel *current_sense = channel->sns_adc_channel;
    assert(current_sense != NULL);
    return hw_adc_getVoltage(current_sense) * ADC_VOLTAGE_TO_CURRENT_A;
}

void io_STloadswitch_reset_set(const ST_LoadSwitch *loadswitch, const bool set)
{
    assert(loadswitch->stby_reset_gpio != NULL);
    hw_gpio_writePin(loadswitch->stby_reset_gpio, set);
}

void io_STloadswitch_Reset(const ST_LoadSwitch *loadswitch)
{
    assert(loadswitch->stby_reset_gpio != NULL);
    hw_gpio_writePin(loadswitch->stby_reset_gpio, false);
    hw_gpio_writePin(loadswitch->stby_reset_gpio, true);
    hw_gpio_writePin(loadswitch->stby_reset_gpio, false);
}

void io_TILoadswitch_Reset(const TI_LoadSwitch *loadSwitch)
{
    assert(loadSwitch->efuse != NULL);
    assert(loadSwitch->efuse->enable_gpio != NULL);

    hw_gpio_writePin(loadSwitch->efuse->enable_gpio, false);
    hw_gpio_writePin(loadSwitch->efuse->enable_gpio, true);
    hw_gpio_writePin(loadSwitch->efuse->enable_gpio, false);
}

bool io_TILoadswitch_pgood(const TI_LoadSwitch *loadSwitch)
{
    assert(loadSwitch->pgood != NULL);
    return hw_gpio_readPin(loadSwitch->pgood);
}
