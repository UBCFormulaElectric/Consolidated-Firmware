#include "app_powerLimiting.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_powerLimiting_datatypes.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_utils.h"
#include "app_vd_datatypes.h"

/************************* Macros *****************************/
#define TOTAL_WHEEL_RPM(fl_rpm, fr_rpm, rl_rpm, rr_rpm) ((double)((fl_rpm) + (fr_rpm) + (rl_rpm) + (rr_rpm)))

/************************* Private Function Prototypes *****************************/
static powerLimiting_inputs app_powerLimiting_getInputs(VD_TorqueToInv torques, VD_WheelRpms wheel_rpms);
static double               app_powerLimiting_calcTorqueReduction(powerLimiting_inputs in, powerLimiting_outputs out);
static void                 app_powerLimiting_broadCast(powerLimiting_outputs out);
static inline double        app_powerLimiting_calcMaxPower(bool isRegen_on);
static inline double        app_powerLimiting_calcTotalPower(VD_TorqueToInv requested_torques, VD_WheelRpms rpms);

/************************* Global Functions *****************************/

powerLimiting_outputs app_powerLimiting(VD_TorqueToInv torques, VD_WheelRpms wheel_rpms)
{
    const powerLimiting_inputs in = app_powerLimiting_getInputs(torques, wheel_rpms);

    const powerLimiting_outputs out = { .total_requested_power =
                                            app_powerLimiting_calcTotalPower(in.torques, in.wheel_rpms),
                                        .power_limit                = app_powerLimiting_calcMaxPower(in.isRegen_on),
                                        .per_wheel_torque_reduction = app_powerLimiting_calcTorqueReduction(in, out) };

    app_powerLimiting_broadCast(out);
    return out;
}

/************************* Private Function Declaration *****************************/
static powerLimiting_inputs app_powerLimiting_getInputs(VD_TorqueToInv requested_torques, VD_WheelRpms wheel_rpms)
{
    const bool                 isRegen_on = SWITCH_ON == app_canRx_CRIT_RegenSwitch_get();
    const powerLimiting_inputs inputs     = {
            .torques         = requested_torques,
            .wheel_rpms      = wheel_rpms,
            .isRegen_on      = isRegen_on,
            .bms_power_limit = isRegen_on ? app_canRx_BMS_ChargePowerLimit_get() : app_canRx_BMS_DischargePowerLimit_get(),
    };

    return inputs;
}

static void app_powerLimiting_broadCast(powerLimiting_outputs out)
{
    app_canTx_VC_TotalRequestedPower_set(out.total_requested_power);
    app_canTx_VC_PowerLimit_set(out.power_limit);
    app_canTx_VC_PerWheelTorqueReduction_set(out.per_wheel_torque_reduction);
}

/************************* Helper Function Declaration *****************************/

static inline double app_powerLimiting_calcTorqueReduction(powerLimiting_inputs in, powerLimiting_outputs out)
{
    // TODO: regen power limiting and regen handling

    // the idea now is that all modulation of torque will mow be the responsibility of the torque allocator alone, there
    // for power limiting, yaw, slip and dynamics est just provide it the info it needs to allocate torque
    double output = 0.0;
    if (out.total_requested_power > in.bms_power_limit)
    {
        const double total_wheel_rpm = TOTAL_WHEEL_RPM(
            in.wheel_rpms.wheel_rpm_fl, in.wheel_rpms.wheel_rpm_fr, in.wheel_rpms.wheel_rpm_rl,
            in.wheel_rpms.wheel_rpm_rr);
        output = POWER_TO_TORQUE(out.total_requested_power - out.power_limit, total_wheel_rpm);
    }

    return output;
}

static inline double app_powerLimiting_calcTotalPower(VD_TorqueToInv requested_torques, VD_WheelRpms rpms)
{
    return TORQUE_TO_POWER(requested_torques.torque_fl, rpms.wheel_rpm_fl) +
           TORQUE_TO_POWER(requested_torques.torque_fr, rpms.wheel_rpm_fr) +
           TORQUE_TO_POWER(requested_torques.torque_rl, rpms.wheel_rpm_rl) +
           TORQUE_TO_POWER(requested_torques.torque_rr, rpms.wheel_rpm_rr);
}

static inline double app_powerLimiting_calcMaxPower(bool isRegen_on)
{
    const double bms_limit =
        isRegen_on ? app_canRx_BMS_ChargePowerLimit_get() : app_canRx_BMS_DischargePowerLimit_get();
    const double abs_p_max = isRegen_on ? POWER_LIMIT_REGEN_kW : RULES_BASED_POWER_LIMIT_KW;
    return fmin(bms_limit, abs_p_max);
}

// Helper functions for compatibility with quintuna API
static float app_totalWheelSpeed(void)
{
    return (
        float)(fabsf((float)app_canRx_INVFL_ActualVelocity_get()) + fabsf((float)app_canRx_INVFR_ActualVelocity_get()) +
               fabsf((float)app_canRx_INVRL_ActualVelocity_get()) + fabsf((float)app_canRx_INVRR_ActualVelocity_get()));
}

float app_powerLimiting_computeMaxPower(bool isRegenOn)
{
    const float bms_power_limit_kW =
        (float)(isRegenOn ? app_canRx_BMS_ChargePowerLimit_get() : app_canRx_BMS_DischargePowerLimit_get());
    const float abs_p_max  = isRegenOn ? POWER_LIMIT_REGEN_kW : RULES_BASED_POWER_LIMIT_KW;
    const float powerLimit = fminf(bms_power_limit_kW, abs_p_max);
    return powerLimit;
}

float app_totalPower(TorqueAllocationOutputs *torques)
{
    return (float)(TORQUE_TO_POWER(torques->front_left_torque, fabsf((float)app_canRx_INVFL_ActualVelocity_get())) +
                   TORQUE_TO_POWER(torques->front_right_torque, fabsf((float)app_canRx_INVFR_ActualVelocity_get())) +
                   TORQUE_TO_POWER(torques->rear_left_torque, fabsf((float)app_canRx_INVRL_ActualVelocity_get())) +
                   TORQUE_TO_POWER(torques->rear_right_torque, fabsf((float)app_canRx_INVRR_ActualVelocity_get())));
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
        (float)CLAMP(inputs->torqueToMotors->front_left_torque, MAX_REGEN_Nm, MAX_TORQUE_REQUEST_NM);
    inputs->torqueToMotors->front_right_torque =
        (float)CLAMP(inputs->torqueToMotors->front_right_torque, MAX_REGEN_Nm, MAX_TORQUE_REQUEST_NM);
    inputs->torqueToMotors->rear_left_torque =
        (float)CLAMP(inputs->torqueToMotors->rear_left_torque, MAX_REGEN_Nm, MAX_TORQUE_REQUEST_NM);
    inputs->torqueToMotors->rear_right_torque =
        (float)CLAMP(inputs->torqueToMotors->rear_right_torque, MAX_REGEN_Nm, MAX_TORQUE_REQUEST_NM);
}