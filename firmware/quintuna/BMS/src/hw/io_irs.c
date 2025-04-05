#include "io_irs.h"

#include "hw_gpios.h"
#include "hw_adcs.h"

#include <math.h>

// Shunt resistor used to measure IR loop current
#define IR_TOTAL_ISENSE_SHUNT_R_OHMS (0.075f)
// Amplifier used to amplify voltage drop across the shunt resistor
#define IR_TOTAL_ISENSE_AMPLIFIER (20.0f)
// Converts voltage measured across shunt resistor to current in A
#define IR_TOTAL_ISENSE_V_TO_A (1.0f / (IR_TOTAL_ISENSE_SHUNT_R_OHMS * IR_TOTAL_ISENSE_AMPLIFIER))
// IR positive closed threshold
#define IR_POSITIVE_CLOSED_THRESH_A (0.5f)

// Gain of the current sense amplifier
#define IR_LOOP_GAIN (1.0f / 20.0f)
// Value of shunt resistor
#define IR_LOOP_SHUNT_RES (1.0f / 75.0e-3f)

bool io_irs_isNegativeClosed(void)
{
    return hw_gpio_readPin(&msd_shdn_sns_pin);
}

void io_irs_closePositive(void)
{
    hw_gpio_writePin(&ir_p_en_pin, true);
}

void io_irs_openPositive(void)
{
    hw_gpio_writePin(&ir_p_en_pin, false);
}

bool io_irs_isPositiveClosed(void)
{
    return hw_gpio_readPin(&ir_p_en_pin);
}

void io_irs_closePrecharge(void)
{
    hw_gpio_writePin(&precharge_en_pin, true);
}

void io_irs_openPrecharge(void)
{
    hw_gpio_writePin(&precharge_en_pin, false);
}

bool io_irs_isPrechargeClosed(void)
{
    return hw_gpio_readPin(&precharge_en_pin);
}
