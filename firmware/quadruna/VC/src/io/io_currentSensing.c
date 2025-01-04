#include "io_currentSensing.h"
#include <stdbool.h>
#include "hw_gpios.h"
#include "hw_adc.h"

// all constants in SI units
#define AMPERAGE_PER_VOLTAGE (1.0f / (5.5e-2f))
#define MIN_VOLTAGE (3.3f / 2.0f)

bool io_currentSensing_hasAccumulatorFault()
{
    return !hw_gpio_readPin(&bat_i_sns_nflt);
}

bool io_currentSensing_hasBatteryFault()
{
    return !hw_gpio_readPin(&acc_i_sns_nflt);
}

float io_currentSensing_getAccumulatorCurrent()
{
    return (hw_adc_getVoltage(ADC1_IN14_BAT_I_SNS) - MIN_VOLTAGE) * AMPERAGE_PER_VOLTAGE;
}

float io_currentSensing_getBatteryCurrent()
{
    return (hw_adc_getVoltage(ADC1_IN5_ACC_I_SENSE) - MIN_VOLTAGE) * AMPERAGE_PER_VOLTAGE;
}
