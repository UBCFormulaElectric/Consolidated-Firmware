#include "app_powerLimiting.h"

#include "app_vehicleDynamicsConstants.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_vehicleDynamics.h"
#include "app_utils.h"

#include <math.h>

static float sumWheelSpeed(void)
{
    return fabsf((float)app_canRx_INVFL_ActualVelocity_get()) + fabsf((float)app_canRx_INVFR_ActualVelocity_get()) +
           fabsf((float)app_canRx_INVRL_ActualVelocity_get()) + fabsf((float)app_canRx_INVRR_ActualVelocity_get());
}

double app_totalPower(const TorqueAllocationOutputs *torques)
{
    return TORQUE_TO_POWER(torques->torque_fl, fabs(app_canRx_INVFL_ActualVelocity_get())) +
           TORQUE_TO_POWER(torques->torque_fr, fabs(app_canRx_INVFR_ActualVelocity_get())) +
           TORQUE_TO_POWER(torques->torque_rl, fabs(app_canRx_INVRL_ActualVelocity_get())) +
           TORQUE_TO_POWER(torques->torque_rr, fabs(app_canRx_INVRR_ActualVelocity_get()));
}

double app_powerLimiting_computeMaxPower(const bool isRegenOn)
{ /**
   *  AMK INVERTER DOES TEMPERATURE BASED LIMITING... USING THAT TEMP > 40 starts derating && TEMP > 60  = inverter off
   */
    // ============== Calculate max powers =================
    // TODO: CONFIRM REGEN POWER LIMIT
    // TODO: LOOK INTO BMS DERATED POWER LIMIT, USE IT TO VALIDATE CURRENT LIMIT
    const double bms_power_limit_kW =
        isRegenOn ? app_canRx_BMS_ChargePowerLimit_get() : app_canRx_BMS_DischargePowerLimit_get();
    const double abs_p_max  = isRegenOn ? POWER_LIMIT_REGEN_kW : RULES_BASED_POWER_LIMIT_KW;
    const double powerLimit = fmin(bms_power_limit_kW, abs_p_max);

    app_canTx_VC_PowerLimitValue_set((float)powerLimit);
    return powerLimit;
}

void app_powerLimiting_torqueReduction(
    const bool               is_regen_mode,
    TorqueAllocationOutputs *torqueToMotors,
    const double             total_requestedPower,
    const double             power_limit,
    const double             derating_value)
{
    const float torque_negative_max_Nm = MIN4(
        torqueToMotors->torque_fl, torqueToMotors->torque_fr, torqueToMotors->torque_rl, torqueToMotors->torque_rr);
    const float avg_max_neg_torque = POWER_TO_TORQUE(power_limit, sumWheelSpeed());

    float scale = CLAMP_TO_ONE(derating_value);

    if (is_regen_mode)
    {
        // If regen torque exceeds limit, scale all torque proportionally
        if (torque_negative_max_Nm < -avg_max_neg_torque)
        {
            scale *= -avg_max_neg_torque / torque_negative_max_Nm;
        }

        torqueToMotors->torque_fl *= scale;
        torqueToMotors->torque_fr *= scale;
        torqueToMotors->torque_rl *= scale;
        torqueToMotors->torque_rr *= scale;
    }
    else if (total_requestedPower > power_limit)
    {
        const float torque_reduction = POWER_TO_TORQUE((total_requestedPower - power_limit), sumWheelSpeed());

        torqueToMotors->torque_fl -= torque_reduction;
        torqueToMotors->torque_fr -= torque_reduction;
        torqueToMotors->torque_rl -= torque_reduction;
        torqueToMotors->torque_rr -= torque_reduction;
    }

    torqueToMotors->torque_fl = CLAMP(torqueToMotors->torque_fl, MAX_REGEN_Nm, MAX_TORQUE_REQUEST_NM);
    torqueToMotors->torque_fr = CLAMP(torqueToMotors->torque_fr, MAX_REGEN_Nm, MAX_TORQUE_REQUEST_NM);
    torqueToMotors->torque_rl = CLAMP(torqueToMotors->torque_rl, MAX_REGEN_Nm, MAX_TORQUE_REQUEST_NM);
    torqueToMotors->torque_rr = CLAMP(torqueToMotors->torque_rr, MAX_REGEN_Nm, MAX_TORQUE_REQUEST_NM);
}
