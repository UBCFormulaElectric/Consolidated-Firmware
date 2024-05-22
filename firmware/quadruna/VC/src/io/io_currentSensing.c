#include "io_currentSensing.h"
#include <stdbool.h>
#include "hw_gpio.h"
#include "hw_adc.h"

// all constants in SI units
#define AMPERAGE_PER_VOLTAGE (1.0f / (5.5e-2f))
#define MIN_VOLTAGE (3.3f / 2.0f)

static const CurrentSensingConfig *config = NULL;

void io_currentSensing_init(const CurrentSensingConfig *current_sensing_config)
{
    config = current_sensing_config;
}

bool io_currentSensing_hasAccumulatorFault()
{
    return !hw_gpio_readPin(&config->acc_fault_gpio);
}

bool io_currentSensing_hasBatteryFault()
{
    return !hw_gpio_readPin(&config->bat_fault_gpio);
}

float io_currentSensing_getAccumulatorCurrent()
{
    return (hw_adc_getVoltage(config->acc_current_adc) - MIN_VOLTAGE) * AMPERAGE_PER_VOLTAGE;
}

float io_currentSensing_getBatteryCurrent()
{
    return (hw_adc_getVoltage(config->bat_current_adc) - MIN_VOLTAGE) * AMPERAGE_PER_VOLTAGE;
}
