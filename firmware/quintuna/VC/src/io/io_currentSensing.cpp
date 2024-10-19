#include "io_currentSensing.h"
#include <stdbool.h>
#include "hw_gpio.h"
#include "hw_adc.h"

// all constants in SI units
constexpr float AMPERAGE_PER_VOLTAGE(1.0f / (5.5e-2f));
constexpr float MIN_VOLTAGE(3.3f / 2.0f);

bool hasAccumulatorFault()
{
    return !hw::gpio::acc_i_sns_nflt.readPin();
}

bool hasBatteryFault()
{
    return !hw::gpio::bat_i_sns_nflt.readPin();
}

float getAccumulatorCurrent()
{
    return (hw::adc::acc_i_sns.getVoltage() - MIN_VOLTAGE) * AMPERAGE_PER_VOLTAGE;
}

float getBatteryCurrent()
{
    return (hw::adc::bat_i_sns.getVoltage() - MIN_VOLTAGE) * AMPERAGE_PER_VOLTAGE;
}
