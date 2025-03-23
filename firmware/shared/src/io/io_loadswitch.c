#include "io_loadswitch.h"
#include "hw_gpio.h"

#define ADC_VOLTAGE_TO_CURRENT_A 1.720f

void io_loadswitch_setChannel(const Efuse *channel, const bool enabled)
{
    if (channel->enable_gpio != NULL)
    {
        hw_gpio_writePin(channel->enable_gpio, enabled);
    }
}

bool io_loadswitch_isChannelEnabled(const Efuse *channel)
{
    return hw_gpio_readPin(channel->enable_gpio);
}

float io_loadswitch_getChannelCurrent(const Efuse *channel)
{
    const AdcChannel *current_sense = channel->sns_adc_channel;
    if (current_sense == NULL)
    {
        return 0;
    }
    return hw_adc_getVoltage(current_sense) * ADC_VOLTAGE_TO_CURRENT_A;
}

void io_STloadswitch_Reset(const ST_LoadSwitch *loadswitch)
{
    hw_gpio_writePin(loadswitch->stby_reset_gpio, true);
    hw_gpio_writePin(loadswitch->stby_reset_gpio, true);
    hw_gpio_writePin(loadswitch->stby_reset_gpio, false);
}

void io_TILoadswitch_Reset(const TI_LoadSwitch *loadSwitch)
{
    if (!hw_gpio_readPin(loadSwitch->pgood))
    {
        hw_gpio_writePin(loadSwitch->efuse->enable_gpio, false);
        hw_gpio_writePin(loadSwitch->efuse->enable_gpio, true);
        hw_gpio_writePin(loadSwitch->efuse->enable_gpio, false);
    }
}