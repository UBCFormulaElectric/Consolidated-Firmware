#include "io_irs.h"
#include "hw_gpios.h"

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

ContactorState io_irs_negativeState(void)
{
    return hw_gpio_readPin(&msd_shdn_sns_pin) ? CONTACTOR_STATE_CLOSED : CONTACTOR_STATE_OPEN;
}

void io_irs_setPositive(const ContactorState state)
{
    hw_gpio_writePin(&ir_p_en_pin, state == CONTACTOR_STATE_CLOSED);
}

ContactorState io_irs_positiveState(void)
{
    return hw_gpio_readPin(&ir_p_en_pin) ? CONTACTOR_STATE_CLOSED : CONTACTOR_STATE_OPEN;
}

void io_irs_setPrecharge(const ContactorState state)
{
    hw_gpio_writePin(&precharge_en_pin, state == CONTACTOR_STATE_CLOSED);
}

ContactorState io_irs_prechargeState(void)
{
    return hw_gpio_readPin(&precharge_en_pin) ? CONTACTOR_STATE_CLOSED : CONTACTOR_STATE_OPEN;
}
