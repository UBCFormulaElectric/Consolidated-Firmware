#include "io_currentSensing.h"
#include <stdbool.h>
#include "hw_gpio.h"
#include "hw_adc.h"

static const CurrentSensingConfig *config;

void io_currentSensing_init(const CurrentSensingConfig *current_sensing_config)
{
    config = current_sensing_config;
}

bool io_currentSensing_hasAccumulatorFault()
{
    return hw_gpio_readPin(&config->acc_fault_gpio);
}

bool io_currentSensing_hasBatteryFault()
{
    return hw_gpio_readPin(&config->bat_fault_gpio);
}

float io_currentSensing_getAccumulatorCurrent()
{
    // TODO: implement voltage to current logic
    return hw_adc_getVoltage(config->acc_current_adc);
}

float io_currentSensing_getBatteryCurrent()
{
    // TODO: implement voltage to current logic
    return hw_adc_getVoltage(config->bat_current_adc);
}
