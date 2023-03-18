#include <math.h>
#include "torquevectoring/App_PowerLimiting.h"
#include "torquevectoring/App_TorqueVectoring.h"
#include "torquevectoring/App_TorqueVectoringConstants.h"

/**
 * @note Computes the max allowable power draw from the battery based on motor temps,
 * the battery power limit (calculated by BMS), and accelerator pedal position.
 * @return A float for the maximum power allowed from the motor,
 */
float App_PowerLimiting_ComputeMaxPower(struct PowerLimiting_Inputs *power_limiting_inputs)
{
    float max_motor_temp = fmaxf(power_limiting_inputs->left_motor_temp_C, power_limiting_inputs->right_motor_temp_C);
    // ============== Calculate max powers =================
    float P_max_motor_temps =
        max_motor_temp > MOTOR_TEMP_CUTOFF_c
            ? POWER_LIMIT_CAR_kW
            : POWER_LIMIT_CAR_kW - (max_motor_temp - MOTOR_TEMP_CUTOFF_c) * MOTOR_TEMP_POWER_DECREMENTING_RATIO;
    float P_max_accelerator = power_limiting_inputs->accelerator_pedal_percent * POWER_LIMIT_CAR_kW;
    // =========== Take min of max powers ==================
    return fminf(
        P_max_motor_temps,
        fminf(power_limiting_inputs->available_battery_power_kW, P_max_accelerator)); // triple min function
}
