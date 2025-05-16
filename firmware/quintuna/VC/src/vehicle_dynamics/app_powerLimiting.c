#include <math.h>
#include "app_powerLimiting.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_canTx.h"
#include "app_canRx.h"

#define RULES_POWER_LIMIT_KW 80.0f

/**
 * Need to add power limiting based on current
 * Need to implement torque request downgrading when requests torque requires a power > 80KW... torque down grading requires nominal motor voltage and nominal motor current IN 
 * 
 */

/**
 * @note Computes the max allowable power draw from the battery based on motor temps,
 * the battery power limit (calculated by BMS), and accelerator pedal position.
 * @return A float for the maximum power allowed from the motor,
 */
float app_powerLimiting_computeMaxPower(struct PowerLimiting_Inputs *inputs)
{
    float max_motor_temp = fmaxf(inputs->left_motor_temp_C, inputs->right_motor_temp_C);

    // ============== Calculate max powers =================
    // 1. Motor Temps 

    float P_max_motor_temps = RULES_POWER_LIMIT_KW; 
    if (max_motor_temp - MOTOR_TEMP_CUTOFF_c >= 30.0f)
    {
        P_max_motor_temps = 0.0;
    }
    else if (max_motor_temp > MOTOR_TEMP_CUTOFF_c)
    {
        P_max_motor_temps =
            RULES_POWER_LIMIT_KW - (max_motor_temp - MOTOR_TEMP_CUTOFF_c) * MOTOR_TEMP_POWER_DECREMENTING_RATIO;
    }

    // 2. Battery state of charge
    float P_max_battery = fminf(RULES_POWER_LIMIT_KW, inputs->current_based_power_limit); // assuming current limit is coming through as a power -- if not convert to power

    // Calculate max power when fully throttled - for debugging purposes, to measure dips in available power
    float P_max = fminf(P_max_motor_temps, P_max_battery);

    app_canTx_VC_PowerLimitValue_set((float)P_max);
    return P_max;
}
