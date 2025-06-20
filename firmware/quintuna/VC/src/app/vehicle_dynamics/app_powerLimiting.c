#include <math.h>
#include "app_powerLimiting.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_vehicleDynamics.h"
#include "app_utils.h"

float app_totalPower(TorqueAllocationOutputs *torques)
{
    return (float)(TORQUE_TO_POWER(torques->front_left_torque, fabsf((float)app_canRx_INVFL_ActualVelocity_get())) +
                   TORQUE_TO_POWER(torques->front_right_torque, fabsf((float)app_canRx_INVFR_ActualVelocity_get())) +
                   TORQUE_TO_POWER(torques->rear_left_torque, fabsf((float)app_canRx_INVRL_ActualVelocity_get())) +
                   TORQUE_TO_POWER(torques->rear_right_torque, fabsf((float)app_canRx_INVRR_ActualVelocity_get())));
}

static float app_totalWheelSpeed()
{
    return (
        float)(fabsf((float)app_canRx_INVFL_ActualVelocity_get()) + fabsf((float)app_canRx_INVFR_ActualVelocity_get()) +
               fabsf((float)app_canRx_INVRL_ActualVelocity_get()) + fabsf((float)app_canRx_INVRR_ActualVelocity_get()));
}

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
    const float bms_power_limit_kW = isRegenOn ? app_canRx_BMS_ChargeCurrentLimit_get() : app_canRx_BMS_DischargeCurrentLimit_get();
    const float abs_p_max = isRegenOn ? POWER_LIMIT_REGEN_kW : RULES_BASED_POWER_LIMIT_KW;
    const float powerLimit = fminf(bms_power_limit_kW,abs_p_max);

    app_canTx_VC_PowerLimitValue_set(powerLimit);
    return powerLimit;
}

float getMaxMotorTemp(void) // -- we're relying solely on the inverter thermal derating -- this is no longer needed but
                            // will likely be used in the future
{
    const float motor_fl_temp = (float)app_canRx_INVFL_MotorTemperature_get();
    const float motor_fr_temp = (float)app_canRx_INVFR_MotorTemperature_get();
    const float motor_rl_temp = (float)app_canRx_INVRL_MotorTemperature_get();
    const float motor_rr_temp = (float)app_canRx_INVRR_MotorTemperature_get();
    return fmaxf(fmaxf(fmaxf(motor_fl_temp, motor_fr_temp), motor_rl_temp), motor_rr_temp);
}

void app_powerLimiting_torqueReduction(PowerLimitingInputs *inputs)
{
    const float torque_negative_max_Nm = MIN4(
        inputs->torqueToMotors->front_left_torque, inputs->torqueToMotors->front_right_torque,
        inputs->torqueToMotors->rear_left_torque, inputs->torqueToMotors->rear_right_torque);
    const float avg_max_neg_torque = POWER_TO_TORQUE(inputs->power_limit, app_totalWheelSpeed());

    float scale = CLAMP_TO_ONE(inputs->derating_value);

    if (inputs->is_regen_mode)
    {
        // If regen torque exceeds limit, scale all torque proportionally
        if (torque_negative_max_Nm < -avg_max_neg_torque)
        {
            scale *= -avg_max_neg_torque / torque_negative_max_Nm;
        }

        inputs->torqueToMotors->front_left_torque *= scale;
        inputs->torqueToMotors->front_right_torque *= scale;
        inputs->torqueToMotors->rear_left_torque *= scale;
        inputs->torqueToMotors->rear_right_torque *= scale;
    }
    else if (inputs->total_requestedPower > inputs->power_limit)
    {
        float torque_reduction =
            POWER_TO_TORQUE((inputs->total_requestedPower - inputs->power_limit), app_totalWheelSpeed());

        inputs->torqueToMotors->front_left_torque -= torque_reduction;
        inputs->torqueToMotors->front_right_torque -= torque_reduction;
        inputs->torqueToMotors->rear_left_torque -= torque_reduction;
        inputs->torqueToMotors->rear_right_torque -= torque_reduction;
    }

    inputs->torqueToMotors->front_left_torque =
        (float)CLAMP(inputs->torqueToMotors->front_left_torque, 0, MAX_TORQUE_REQUEST_NM);
    inputs->torqueToMotors->front_right_torque =
        (float)CLAMP(inputs->torqueToMotors->front_right_torque, 0, MAX_TORQUE_REQUEST_NM);
    inputs->torqueToMotors->rear_left_torque =
        (float)CLAMP(inputs->torqueToMotors->rear_left_torque, 0, MAX_TORQUE_REQUEST_NM);
    inputs->torqueToMotors->rear_right_torque =
        (float)CLAMP(inputs->torqueToMotors->rear_right_torque, 0, MAX_TORQUE_REQUEST_NM);
}
