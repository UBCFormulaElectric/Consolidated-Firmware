#include <stdlib.h>
#include "math.h"

#include "app_regen.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_powerLimiting.h"
#include "app_activeDifferential.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_units.h"
#include "app_utils.h"

/**
 * Stop regen < 5.0km/hr and start regen > 7.0km/hr
 * @param ActiveDifferential_Inputs struct to populate data
 * @return true if wheel speed meets this condition, false
 * otherwise
 */
static bool wheelSpeedInRange(ActiveDifferential_Inputs *inputs);

/**
 * Check if battery cells are less than 4.1V
 * @param RegenBraking_Inputs struct to populate data
 * @return true battery cells meet this condition,
 * false otherwise
 */
static bool batteryLevelInRange(RegenBraking_Inputs *regenAttr);

/**
 * Algorithm to send negative torque request dependent
 * on accelerator pedal percentage in range [-100, 0]
 * and to do active differential or not
 * @param inputs are inputs for active differential
 * @param regenAttr struct provides for torque request
 */
static void computeRegenTorqueRequest(
    ActiveDifferential_Inputs *inputs,
    RegenBraking_Inputs       *regenAttr,
    PowerLimiting_Inputs      *powerInputs);

// Global variables for regenerative braking logic
static RegenBraking_Inputs       regenAttributes = { .enable_active_differential = true };
static ActiveDifferential_Inputs activeDifferentialInputs;
static PowerLimiting_Inputs      powerLimitingInputs = { .regen_power_limit_kW = POWER_LIMIT_REGEN_kW };
static bool                      regen_enabled       = true;

void app_regen_init(void)
{
    app_canTx_VC_RegenEnabled_set(true);
    app_canTx_VC_Warning_RegenNotAvailable_set(false);
}

void app_regen_run(float accelerator_pedal_percentage)
{
    // pedal percentage = [-1.0f, 0.0f] for deceleration range
    activeDifferentialInputs.accelerator_pedal_percentage = accelerator_pedal_percentage;
    bool regen_available = app_regen_safetyCheck(&regenAttributes, &activeDifferentialInputs);

    if (regen_available)
    {
        computeRegenTorqueRequest(&activeDifferentialInputs, &regenAttributes, &powerLimitingInputs);
    }
    else
    {
        regenAttributes.torque_rr_Nm = 0.0f;
        regenAttributes.torque_rl_Nm = 0.0f;
        regenAttributes.torque_fr_Nm = 0.0f;
        regenAttributes.torque_fl_Nm = 0.0f;
    }

    app_canTx_VC_RegenEnabled_set(regen_available);
    app_canTx_VC_Warning_RegenNotAvailable_set(!regen_available);

    app_regen_sendTorqueRequest(&regenAttributes);
}

bool app_regen_safetyCheck(RegenBraking_Inputs *regenAttr, ActiveDifferential_Inputs *inputs)
{
    bool battery_temp_in_range = app_canRx_BMS_MaxCellTemp_get() < MAX_BATTERY_TEMP;
    return battery_temp_in_range && wheelSpeedInRange(inputs) && batteryLevelInRange(regenAttr);
}

static bool wheelSpeedInRange(ActiveDifferential_Inputs *inputs)
{
    inputs->motor_speed_rr_rpm = (float)app_canRx_INVRR_ActualVelocity_get();
    inputs->motor_speed_rl_rpm = (float)app_canRx_INVRL_ActualVelocity_get();
    inputs->motor_speed_fr_rpm = (float)app_canRx_INVFR_ActualVelocity_get();
    inputs->motor_speed_fl_rpm = (float)app_canRx_INVFL_ActualVelocity_get();

    // Hysterisis

    float min_motor_speed = MOTOR_RPM_TO_KMH(MIN4(
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
    PowerLimiting_Inputs      *powerInputs)
{
    float pedal_percentage = activeDiffInputs->accelerator_pedal_percentage;

    powerInputs->accelerator_pedal_percent = -pedal_percentage; // power limiting function requires positive pedal value
    // powerInputs->left_motor_temp_C         = app_canRx_INVL_MotorTemperature_get();
    // powerInputs->right_motor_temp_C        = app_canRx_INVR_MotorTemperature_get();

    regenAttr->derating_value = 1.0f;

    if (regenAttr->battery_level > 3.9f)
    {
        regenAttr->derating_value = SOC_LIMIT_DERATING_VALUE;
    }

    float min_motor_speed_kmh = MOTOR_RPM_TO_KMH(MIN4(
        activeDiffInputs->motor_speed_rr_rpm, activeDiffInputs->motor_speed_rl_rpm,
        activeDiffInputs->motor_speed_fr_rpm, activeDiffInputs->motor_speed_fl_rpm));

    if (min_motor_speed_kmh < 10.0f)
    {
        regenAttr->derating_value = regenAttr->derating_value * (min_motor_speed_kmh - SPEED_MIN_kph) / SPEED_MIN_kph;
    }

    activeDiffInputs->power_max_kW    = app_powerLimiting_computeMaxPower(powerInputs->current_based_power_limit_kW);
    activeDiffInputs->wheel_angle_deg = app_canRx_FSM_SteeringAngle_get() * APPROX_STEERING_TO_WHEEL_ANGLE;

    if (regenAttr->enable_active_differential)
    {
        app_regen_computeActiveDifferentialTorque(activeDiffInputs, regenAttr);
    }
    else
    {
        // no power limit, no active differential
        float regen_torque_request = MAX_REGEN_Nm * pedal_percentage * regenAttr->derating_value;
        regenAttr->torque_rr_Nm    = regen_torque_request;
        regenAttr->torque_rl_Nm    = regen_torque_request;
        regenAttr->torque_fr_Nm    = regen_torque_request;
        regenAttr->torque_fl_Nm    = regen_torque_request;
    }
}

void app_regen_computeActiveDifferentialTorque(ActiveDifferential_Inputs *inputs, RegenBraking_Inputs *regenAttr)
{
    float Delta = app_activeDifferential_wheelAngleToSpeedDelta(inputs->wheel_angle_deg);

    float cl = (1 + Delta);
    float cr = (1 - Delta);

    float torque_limit_Nm = -app_activeDifferential_powerToTorque(
        inputs->power_max_kW, inputs->motor_speed_fl_rpm, inputs->motor_speed_fr_rpm, inputs->motor_speed_rl_rpm,
        inputs->motor_speed_rr_rpm, cl, cr);

    float torque_left_Nm         = torque_limit_Nm * cl;
    float torque_right_Nm        = torque_limit_Nm * cr;
    float torque_negative_max_Nm = fminf(torque_left_Nm, torque_right_Nm);

    float scale = CLAMP_TO_ONE(regenAttr->derating_value);
    if (torque_negative_max_Nm < MAX_REGEN_Nm)
    {
        scale *= MAX_REGEN_Nm / torque_negative_max_Nm;
    }

    regenAttr->torque_rr_Nm = torque_right_Nm * scale;
    regenAttr->torque_rl_Nm = torque_left_Nm * scale;
    regenAttr->torque_fr_Nm = torque_right_Nm * scale;
    regenAttr->torque_fl_Nm = torque_left_Nm * scale;
}

void app_regen_sendTorqueRequest(RegenBraking_Inputs *regenAttr)
{
    app_canTx_VC_INVFLTorqueSetpoint_set(PEDAL_REMAPPING(regenAttr->torque_fl_Nm));
    app_canTx_VC_INVFRTorqueSetpoint_set(PEDAL_REMAPPING(regenAttr->torque_fr_Nm));
    app_canTx_VC_INVRLTorqueSetpoint_set(PEDAL_REMAPPING(regenAttr->torque_rl_Nm));
    app_canTx_VC_INVRRTorqueSetpoint_set(PEDAL_REMAPPING(regenAttr->torque_rr_Nm));
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
