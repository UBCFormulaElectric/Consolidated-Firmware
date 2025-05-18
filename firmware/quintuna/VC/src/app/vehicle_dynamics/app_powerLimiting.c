#include <math.h>
#include "app_powerLimiting.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_canTx.h"
#include "app_canRx.h"

#define RULE_BASED_POWER_LIMIT_KW 80.0f

/**
 * @note Computes the max allowable power draw from the battery based on motor temps,
 * the battery power limit (calculated by BMS), and accelerator pedal position.
 * @return A float for the maximum power allowed from the motor,
 */
float app_powerLimiting_computeMaxPower(struct PowerLimiting_Inputs *inputs)
{

    /**
     *  AMK INVERTER DOES TEMPERATURE BASED POWER LIMITING... WE LIKELY CAN JUST USE THIS SEE PAGE 84 
     */

    // Calculate max power when fully throttled - for debugging purposes, to measure dips in available power
    float P_max = fminf(RULE_BASED_POWER_LIMIT_KW, inputs->current_based_power_limit_kW);
    app_canTx_VC_PowerLimitValue_set((float)P_max);

    return P_max;
}
