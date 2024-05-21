#include "io_airs.h"

#include <math.h>

// Shunt resistor used to measure AIR loop current
#define AIR_TOTAL_ISENSE_SHUNT_R_OHMS (0.075f)
// Amplifier used to amplify voltage drop across the shunt resistor
#define AIR_TOTAL_ISENSE_AMPLIFIER (20.0f)
// Converts voltage measured across shunt resistor to current in A
#define AIR_TOTAL_ISENSE_V_TO_A (1.0f / (AIR_TOTAL_ISENSE_SHUNT_R_OHMS * AIR_TOTAL_ISENSE_AMPLIFIER))
// AIR positive closed threshold
#define AIR_POSITIVE_CLOSED_THRESH_A (0.5f)

// Gain of the current sense amplifier
#define AIR_LOOP_GAIN (1.0f / 20.0f)
// Value of shunt resistor
#define AIR_LOOP_SHUNT_RES (1.0f / 75.0e-3f)

static const AirsConfig *config = NULL;

void io_airs_init(const AirsConfig *airs_config)
{
    config = airs_config;
}

bool io_airs_isNegativeClosed(void)
{
    return hw_gpio_readPin(&config->air_n_gpio);
}

void io_airs_closePositive(void)
{
    hw_gpio_writePin(&config->air_p_gpio, true);
}

void io_airs_openPositive(void)
{
    hw_gpio_writePin(&config->air_p_gpio, false);
}

bool io_airs_isPositiveClosed(void)
{
    return hw_gpio_readPin(&config->air_p_gpio);
}

void io_airs_closePrecharge(void)
{
    hw_gpio_writePin(&config->precharge_gpio, true);
}

void io_airs_openPrecharge(void)
{
    hw_gpio_writePin(&config->precharge_gpio, false);
}

bool io_airs_isPrechargeClosed(void)
{
    return hw_gpio_readPin(&config->precharge_gpio);
}

float io_airs_getLoopCurrent()
{
    const float adc_voltage = hw_adc_getVoltage(config->loop_isense_channel);

    if (adc_voltage < 0.0f)
    {
        return NAN;
    }

    //   +-------<0.075>-------+
    //   |                     |
    //   |  +---------------+  |
    //   +--| Current Sense |--+
    //      |   Amplifier   |------
    //      +---------------+
    //
    // Gain = 20V/V
    //                   ADC Voltage
    // Current = -----------------------------
    //              Sense Resistance * Gain

    // return the current corresponding to the ADC voltage
    return adc_voltage * AIR_LOOP_SHUNT_RES * AIR_LOOP_GAIN;
}
