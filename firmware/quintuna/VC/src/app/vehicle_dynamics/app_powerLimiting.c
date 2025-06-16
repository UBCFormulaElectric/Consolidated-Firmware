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
float app_powerLimiting_computeMaxPower(const bool isRegenOn)
{ /**
   *  AMK INVERTER DOES TEMPERATURE BASED LIMITING... USING THAT TEMP > 40 starts derating && TEMP > 60  = inverter off
   */
    // ============== Calculate max powers =================
    // TODO: CONFIRM REGEN POWER LIMIT
    // TODO: LOOK INTO BMS DERATED POWER LIMIT, USE IT TO VALIDATE CURRENT LIMIT
    const float current_based_power_limit_kW =
        app_canRx_BMS_TractiveSystemVoltage_get() * app_canRx_BMS_DischargeCurrentLimit_get();
    const float P_max =
        fminf(isRegenOn ? POWER_LIMIT_REGEN_kW : RULES_BASED_POWER_LIMIT_KW, current_based_power_limit_kW);

    app_canTx_VC_PowerLimitValue_set(P_max);
    return P_max;
}

float getMaxMotorTemp(void) // not used anywhere???
{
    const float motor_fl_temp = app_canRx_INVRL_MotorTemperature_get();
    const float motor_fr_temp = app_canRx_INVFL_MotorTemperature_get();
    const float motor_rl_temp = app_canRx_INVRL_MotorTemperature_get();
    const float motor_rr_temp = app_canRx_INVRR_MotorTemperature_get();
    return fmaxf(fmaxf(fmaxf(motor_fl_temp, motor_fr_temp), motor_rl_temp), motor_rr_temp);
}