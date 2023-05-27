#include <math.h>
#include "torquevectoring/App_PowerLimiting.h"
#include "torquevectoring/App_TorqueVectoring.h"
#include "torquevectoring/App_TorqueVectoringConstants.h"

/**
 * @note Computes the max allowable power draw from the battery based on motor temps,
 * the battery power limit (calculated by BMS), and accelerator pedal position.
 * @return A float for the maximum power allowed from the motor,
 */
float App_PowerLimiting_ComputeMaxPower(struct PowerLimiting_Inputs *inputs)
{
    float max_motor_temp = fmaxf(inputs->left_motor_temp_C, inputs->right_motor_temp_C);

    // ============== Calculate max powers =================
    // 1. Motor Temps
    float P_max_motor_temps = POWER_LIMIT_CAR_kW;
    if ((float)max_motor_temp - (float)MOTOR_TEMP_CUTOFF_c >= (float)30.0)
    {
        P_max_motor_temps = 0.0;
    }
    else if (max_motor_temp > MOTOR_TEMP_CUTOFF_c)
    {
        P_max_motor_temps =
            POWER_LIMIT_CAR_kW - (max_motor_temp - MOTOR_TEMP_CUTOFF_c) * MOTOR_TEMP_POWER_DECREMENTING_RATIO;
    }

    // 2. Battery state of charge
    float P_max_battery = inputs->available_battery_power_kW;

    // 3. Pedal percentage
    float P_max_accelerator = inputs->accelerator_pedal_percent * POWER_LIMIT_CAR_kW;

    // Calculate max power when fully throttled - for debugging purposes, to measure dips in available power
    float P_max_full_throttle = fminf(P_max_motor_temps, P_max_battery);
    App_CanTx_DCM_Debug_PowerLimit_PowerLimit_AtFullThrottle_Set(P_max_full_throttle);

    float P_max = fminf(P_max_full_throttle, P_max_accelerator);
    App_CanTx_DCM_Debug_PowerLimit_PowerLimit(P_max);

    return P_max;
}
