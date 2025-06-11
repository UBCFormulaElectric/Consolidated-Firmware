#include <math.h>
#include "app_powerLimiting.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_canTx.h"
#include "app_canRx.h"

// #define RULE_BASED_POWER_LIMIT_KW 80.0f

// /**
//  * @note Computes the max allowable power draw from the battery based on motor temps,
//  * the battery power limit (calculated by BMS), and accelerator pedal position.
//  * @return A float for the maximum power allowed from the motor,
//  */
// float app_powerLimiting_computeMaxPower(struct PowerLimiting_Inputs *inputs)
// {

//     /**
//      *  AMK INVERTER DOES TEMPERATURE BASED POWER LIMITING.. WE LIKELY CAN JUST USE THIS SEE PAGE 84 but using below
//      for redundancy
//      */

//     // ============== Calculate max powers =================
//     // 1. Motor Temps
//     float P_max_motor_temps = RULE_BASED_POWER_LIMIT_KW;
//     if (max_motor_temp - MOTOR_TEMP_CUTOFF_c >= 30.0f)
//     {
//         P_max_motor_temps = 0.0;
//     }
//     else if (max_motor_temp > MOTOR_TEMP_CUTOFF_c)
//     {
//         P_max_motor_temps = RULE_BASED_POWER_LIMIT_KW - (max_motor_temp - MOTOR_TEMP_CUTOFF_c) *
//         MOTOR_TEMP_POWER_DECREMENTING_RATIO;
//     }

//     // Calculate max power when fully throttled - for debugging purposes, to measure dips in available power
//     float P_max =(fminf(fminf(RULE_BASED_POWER_LIMIT_KW, inputs->current_based_power_limit_kW), P_max_motor_temps));
//     app_canTx_VC_PowerLimitValue_set((float)P_max);

//     return P_max;
// }
