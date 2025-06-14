#include <math.h>
#include "app_powerLimiting.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_canTx.h"
#include "app_canRx.h"

/**
 * @note Computes the max allowable power draw from the battery based on motor temps,
 * the battery power limit (calculated by BMS), and accelerator pedal position.
 * @return A float for the maximum power allowed from the motor,
 */
float app_powerLimiting_computeMaxPower(bool isRegenOn)
{ /**
   *  AMK INVERTER DOES TEMPERATURE BASED LIMITING... USING THAT TEMP > 40 starts derating && TEMP > 60  = inverter off
   */

    // ============== Calculate max powers =================

    float current_based_power_limit_kW;
    float P_max;

    // TODO: CONFIRM REGEN POWER LIMIT
    // TODO: LOOK INTO BMS DERATED POWER LIMIT, USE IT TO VALIDATE CURRENT LIMIT

    if (isRegenOn)
    {
        // TODO: CHANGE THIS PLZZ
        // current_based_power_limit_kW = app_canRx_BMS_TractiveSystemVoltage_get() *
        // app_canRx_BMS_ChargeCurrentLimit_get();
        current_based_power_limit_kW = 0.0f;
        P_max                        = fminf(POWER_LIMIT_REGEN_kW, current_based_power_limit_kW);
    }
    else
    {
        // TODO: CHANGE THIS PLZZ
        // current_based_power_limit_kW =
        //     app_canRx_BMS_TractiveSystemVoltage_get() * app_canRx_BMS_DischargeCurrentLimit_get();
        current_based_power_limit_kW = 0.0f;
        P_max                        = fminf(RULES_BASED_POWER_LIMIT_KW, current_based_power_limit_kW);
    }

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