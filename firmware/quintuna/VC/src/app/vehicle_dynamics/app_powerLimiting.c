#include <math.h>
#include "app_powerLimiting.h"
#include "app_vehicleDynamicsConstants.h"

#include "app_canTx.h"
#include "app_canRx.h"

static float getMaxMotorTemp(void);

/**
 * @note Computes the max allowable power draw from the battery based on motor temps,
 * the battery power limit (calculated by BMS), and accelerator pedal position.
 * @return A float for the maximum power allowed from the motor,
 */
float app_powerLimiting_computeMaxPower(float current_based_power_limit_kW)
{ /**
   *  AMK INVERTER DOES TEMPERATURE BASED LIMITING... USING THAT TEMP > 40 starts derating && TEMP > 60  = inverter off
   */

    // ============== Calculate max powers =================

    // Calculate max power when fully throttled - for debugging purposes, to measure dips in available power
    float P_max = fminf(RULES_BASED_POWER_LIMIT_KW, current_based_power_limit_kW);
    app_canTx_VC_PowerLimitValue_set((float)P_max);

    return P_max;
}

float getMaxMotorTemp(void)
{
    float motor_fl_temp = app_canRx_INVRL_MotorTemperature_get();
    float motor_fr_temp = app_canRx_INVFL_MotorTemperature_get();
    float motor_rl_temp = app_canRx_INVRL_MotorTemperature_get();
    float motor_rr_temp = app_canRx_INVRR_MotorTemperature_get();

    float max_motor_temp = fmaxf(fmaxf(fmaxf(motor_fl_temp, motor_fr_temp), motor_rl_temp), motor_rr_temp);

    return max_motor_temp;
}