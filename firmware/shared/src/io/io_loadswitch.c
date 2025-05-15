#include "io_loadswitch.h"
#include "hw_gpio.h"
#include <stdbool.h>
#include <assert.h>

#define ADC_VOLTAGE_TO_CURRENT_A 1.720f
#define CURRENT_THRESH 0.025f

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

bool io_TILoadswitch_Status(const TI_LoadSwitch *loadswitch)
{
    assert(loadswitch->pgood != NULL);
    return hw_gpio_readPin(loadswitch->pgood);
}

bool io_STLoadswitch_Status(const ST_LoadSwitch *loadswitch)
{
    assert(loadswitch->efuse1 != NULL && loadswitch->efuse2 != NULL);
    assert(loadswitch->efuse1->sns_adc_channel != NULL && loadswitch->efuse2->sns_adc_channel);

    if (io_loadswitch_isChannelEnabled(loadswitch->efuse1) &&
        io_loadswitch_getChannelCurrent(loadswitch->efuse1) <= CURRENT_THRESH)
    {
        return false;
    }

    if (io_loadswitch_isChannelEnabled(loadswitch->efuse2) &&
        io_loadswitch_getChannelCurrent(loadswitch->efuse2) <= CURRENT_THRESH)
    {
        return false;
    }

    return true;
}