#include <app_canAlerts.h>
#include <stdlib.h>
#include "math.h"

#include "app_regen.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_powerLimiting.h"
#include "app_torqueDistribution.h"
#include "app_activeDifferential.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_units.h"
#include "app_utils.h"

/**
 * Stop regen < 5.0km/hr and start regen > 7.0km/hr
 * @param inputs struct to populate data
 * @return true if wheel speed meets this condition, false
 * otherwise
 */
static bool wheelSpeedInRange(ActiveDifferential_Inputs *inputs);

/**
 * Check if battery cells are less than 4.1V
 * @param regenAttr struct to populate data
 * @return true battery cells meet this condition,
 * false otherwise
 */
static bool batteryLevelInRange(RegenBraking_Inputs *regenAttr);

/**
 * Algorithm to send negative torque request dependent
 * on accelerator pedal percentage in range [-100, 0]
 * and to do active differential or not
 * @param activeDiffInputs
 * @param regenAttr struct provides for torque request
 * @param torqueOutputToMotors
 */
static void computeRegenTorqueRequest(
    ActiveDifferential_Inputs *activeDiffInputs,
    RegenBraking_Inputs       *regenAttr,
    TorqueAllocationOutputs   *torqueOutputToMotors);

// Global variables for regenerative braking logic
static RegenBraking_Inputs        regenAttributes = { .enable_active_differential = true };
static ActiveDifferential_Inputs  activeDifferentialInputs;
static ActiveDifferential_Outputs activeDifferentialOutputs;
static bool                       regen_enabled = true;
static PowerLimitingInputs        powerLimitingInputs;

void app_regen_run(const float accelerator_pedal_percentage, TorqueAllocationOutputs *torqueOutputToMotors)
{
    // pedal percentage = [-1.0f, 0.0f] for deceleration range
    activeDifferentialInputs.accelerator_pedal_percentage = accelerator_pedal_percentage;
    const bool regen_available = app_regen_safetyCheck(&regenAttributes, &activeDifferentialInputs);
    if (regen_available)
    {
        computeRegenTorqueRequest(&activeDifferentialInputs, &regenAttributes, torqueOutputToMotors);
    }
    else
    {
        torqueOutputToMotors->front_left_torque  = 0.0f;
        torqueOutputToMotors->front_right_torque = 0.0f;
        torqueOutputToMotors->rear_left_torque   = 0.0f;
        torqueOutputToMotors->rear_right_torque  = 0.0f;
    }

    app_canTx_VC_RegenEnabled_set(regen_available);
    app_canAlerts_VC_Info_RegenNotAvailable_set(!regen_available);
}

bool app_regen_safetyCheck(RegenBraking_Inputs *regenAttr, ActiveDifferential_Inputs *inputs)
{
    const bool battery_temp_in_range = app_canRx_BMS_MaxCellTemp_get() < MAX_BATTERY_TEMP;
    return battery_temp_in_range && wheelSpeedInRange(inputs) && batteryLevelInRange(regenAttr);
}

static bool wheelSpeedInRange(ActiveDifferential_Inputs *inputs)
{
    inputs->motor_speed_rr_rpm = fabsf((float)app_canRx_INVRR_ActualVelocity_get());
    inputs->motor_speed_rl_rpm = fabsf((float)app_canRx_INVRL_ActualVelocity_get());
    inputs->motor_speed_fr_rpm = fabsf((float)app_canRx_INVFR_ActualVelocity_get());
    inputs->motor_speed_fl_rpm = fabsf((float)app_canRx_INVFL_ActualVelocity_get());

    // Hysterisis

    const float min_motor_speed = MOTOR_RPM_TO_KMH(MIN4(
        inputs->motor_speed_rr_rpm, inputs->motor_speed_rl_rpm, inputs->motor_speed_fr_rpm,
        inputs->motor_speed_fl_rpm));

    if (!regen_enabled && min_motor_speed > 7.0f)
    {
        regen_enabled = true;
    }
    else if (regen_enabled && min_motor_speed <= SPEED_MIN_kph)
    {
        regen_enabled = false;
    }

    return regen_enabled;
}

static bool batteryLevelInRange(RegenBraking_Inputs *regenAttr)
{
    regenAttr->battery_level = app_canRx_BMS_MaxCellVoltage_get();
    return regenAttr->battery_level < 4.1f;
}

static void computeRegenTorqueRequest(
    ActiveDifferential_Inputs *activeDiffInputs,
    RegenBraking_Inputs       *regenAttr,
    TorqueAllocationOutputs   *torqueOutputToMotors)
{
    const float min_motor_speed_kmh = MOTOR_RPM_TO_KMH(MIN4(
        activeDiffInputs->motor_speed_rr_rpm, activeDiffInputs->motor_speed_rl_rpm,
        activeDiffInputs->motor_speed_fr_rpm, activeDiffInputs->motor_speed_fl_rpm));

    regenAttr->derating_value = 1.0f;

    if (min_motor_speed_kmh < 10.0f)
    {
        regenAttr->derating_value = (min_motor_speed_kmh - SPEED_MIN_kph) / SPEED_MIN_kph;
    }
    if (regenAttr->battery_level > 3.9f)
    {
        // TODO surely  smarter? and or more derate as SOC gets too high
        regenAttr->derating_value *= SOC_LIMIT_DERATING_VALUE;
    }

    if (regenAttr->enable_active_differential)
    {
        activeDiffInputs->power_max_kW        = app_powerLimiting_computeMaxPower(regen_enabled);
        activeDiffInputs->wheel_angle_deg     = app_canRx_FSM_SteeringAngle_get() * APPROX_STEERING_TO_WHEEL_ANGLE;
        activeDiffInputs->requested_torque_Nm = MAX_REGEN_Nm * activeDiffInputs->accelerator_pedal_percentage;

        app_activeDifferential_computeTorque(activeDiffInputs, torqueOutputToMotors);

        powerLimitingInputs.derating_value       = regenAttr->derating_value;
        powerLimitingInputs.power_limit          = activeDiffInputs->power_max_kW;
        powerLimitingInputs.is_regen_mode        = regen_enabled;
        powerLimitingInputs.torqueToMotors       = torqueOutputToMotors;
        powerLimitingInputs.total_requestedPower = app_totalPower(torqueOutputToMotors);
        app_powerLimiting_torqueReduction(&powerLimitingInputs);
    }
    else
    {
        // no power limit, no active differential
        const float regen_torque_request =
            MAX_REGEN_Nm * activeDiffInputs->accelerator_pedal_percentage * regenAttr->derating_value;
        torqueOutputToMotors->front_left_torque  = regen_torque_request;
        torqueOutputToMotors->front_right_torque = regen_torque_request;
        torqueOutputToMotors->rear_left_torque   = regen_torque_request;
        torqueOutputToMotors->rear_right_torque  = regen_torque_request;
    }
}

float app_regen_pedalRemapping(float apps_pedal_percentage)
{
    apps_pedal_percentage = (apps_pedal_percentage - PEDAL_SCALE);

    if (apps_pedal_percentage < 0.0f)
    {
        return apps_pedal_percentage / PEDAL_SCALE;
    }
    else if (apps_pedal_percentage <= 0.1f)
    {
        return 0.0f;
    }
    else
    {
        return (apps_pedal_percentage - 0.1f) / (MAX_PEDAL_PERCENT - PEDAL_SCALE - 0.1f);
    }
}
